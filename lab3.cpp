#include <pugixml.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>


struct TransportDataItem {
    std::wstring name;
    double value;
};

struct TransportStation {
    std::wstring name;
    double xCoordinate;
    double yCoordinate;
    bool visited = false;

    bool operator<(const TransportStation& anotherStation) const
    {
        return name < anotherStation.name;
    }

    bool operator!=(const TransportStation& anotherStation) const
    {
        return name != anotherStation.name;
    }
};


class TransportDataAnalyzer {
    wchar_t *fileName;
    pugi::xml_document document;
    pugi::xml_parse_result xmlParseResult;
    
    std::vector<std::wstring> vehicles;
    std::map<std::wstring, TransportDataItem> routesWithMaximumStations;
    std::map<std::wstring, TransportDataItem> longestRoutes;
    TransportDataItem streetWithMaximumStations;


    std::vector<std::wstring> splitStringByComma(const std::wstring& str);
    std::vector<std::wstring> splitStringByDelimiters(const std::wstring& str, const std::wstring& delimiters);

    double distanceBetweenStations(const TransportStation& station1, const TransportStation& station2)
    {
        return sqrt( (station1.xCoordinate - station2.xCoordinate) * (station1.xCoordinate - station2.xCoordinate) + 
                     (station1.yCoordinate - station2.yCoordinate) * (station1.yCoordinate - station2.yCoordinate));
    }

    void findVehicles();
    void findRoutesWithMaximumStations();
    void findLongestRoutes();
    void findStreetWithMaximumStations();

public:

    void outputData();
    std::map<std::wstring, TransportDataItem> getRoutesWithMaximumStations()
    {
        auto m(routesWithMaximumStations);
        return m;
    }
    std::map<std::wstring, TransportDataItem> getLongestRoutes()
    {
        auto m(longestRoutes);
        return m;
    }
    TransportDataItem getStreetWithMaximumStations()
    {
        auto street(streetWithMaximumStations);
        return street;
    }

    TransportDataAnalyzer(const wchar_t *fileName);
   
    void analyze();
    void loadXmlData(const wchar_t *fileName);
};


















std::vector<std::wstring> TransportDataAnalyzer::splitStringByComma(const std::wstring& str) {
    std::vector<std::wstring> vect;
    std::wstringstream stringStream(str);


    while (stringStream.good())
    {
        std::wstring substr;
        getline(stringStream, substr, L',');
        vect.push_back(substr);
    }

    return vect;
}

std::vector<std::wstring> TransportDataAnalyzer::splitStringByDelimiters(const std::wstring& str, const std::wstring& delimiters) {
    std::vector<std::wstring> vect;
    int begin = 0;
    int currentPosition = 0;

    std::wstring currentString;
    while ((begin = str.find_first_not_of(delimiters, currentPosition)) != std::wstring::npos)
    {
        currentPosition = str.find_first_of(delimiters, begin + 1);
        currentString = str.substr(begin, currentPosition - begin);
        if (currentString.at(0) == L' ')
            currentString = currentString.substr(1);

        vect.push_back(currentString);

    }

    return vect;
}

void TransportDataAnalyzer::findVehicles() {
    _setmode(_fileno(stdout), _O_U16TEXT);

    for (pugi::xml_node currentStation : document.child(L"dataset").children(L"transport_station"))
    {
        pugi::xml_node currentStationVehicle = currentStation.child(L"type_of_vehicle");
        std::wstring currentStationVehicleString = currentStationVehicle.text().as_string();

        if (std::find(vehicles.begin(), vehicles.end(), currentStationVehicleString) == vehicles.end())
            vehicles.push_back(currentStationVehicleString);
  
    }
}

void TransportDataAnalyzer::findRoutesWithMaximumStations()
{
    _setmode(_fileno(stdout), _O_U16TEXT);



    // initialize maximums
    for (auto vehicle : vehicles)
    {
        TransportDataItem item = { vehicle, INT_MIN };
        routesWithMaximumStations[vehicle] = item;
    }

    // initialize map
    std::map<std::wstring, std::map<std::wstring, int>> maps;


    // iterate through all stations
    for (pugi::xml_node currentStation : document.child(L"dataset").children(L"transport_station"))
    {
        std::wstring currentStationVehicle = currentStation.child(L"type_of_vehicle").text().as_string();
        std::vector<std::wstring> currentStationRoutes = splitStringByDelimiters(currentStation.child_value(L"routes"), L",. ");

        // increment routes
        for (auto route : currentStationRoutes)
        {
            maps[currentStationVehicle][route]++;
        }
    }

    // evaluate maximums
    for (auto vehicle : maps)
    {
        for (auto route : vehicle.second)
        {
            if (route.second > routesWithMaximumStations[vehicle.first].value)
            {
                routesWithMaximumStations[vehicle.first].name = route.first;
                routesWithMaximumStations[vehicle.first].value = route.second;
            }
        }

    }
}



void TransportDataAnalyzer::findLongestRoutes()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    // initialize map
    std::map<std::wstring, std::map<std::wstring, std::set<TransportStation>>> routesMap;
    

    // iterate through all stations
    for (pugi::xml_node currentStation : document.child(L"dataset").children(L"transport_station"))
    {
        std::wstring currentStationVehicle = currentStation.child(L"type_of_vehicle").text().as_string();
        std::wstring currentStationName = currentStation.child(L"the_official_name").text().as_string();
        std::vector<std::wstring> currentStationRoutes = splitStringByDelimiters(currentStation.child_value(L"routes"), L",. ");
        std::vector<std::wstring> currentStationCoordinates = splitStringByDelimiters(currentStation.child(L"coordinates").text().as_string(), L", ");
        TransportStation station = { currentStationName, stod(currentStationCoordinates[0], NULL), stod(currentStationCoordinates[1], NULL) };

        for (auto route : currentStationRoutes)
        {
            routesMap[currentStationVehicle][route].insert(station);
        }
    }


    // iterate through all vehicles
    for (auto vehicle : routesMap)
    {
        // iterate through all routes and find maximum
        double maximumRouteLength = INT_MIN;

        for (auto route : vehicle.second)
        {

            // find two stations with maximum distance
            double maximumDistance = INT_MIN;
            TransportStation maxStation1, maxStation2;


            double currentDistance;
            for (auto station1 : route.second)
            {
                for (auto station2 : route.second)
                {
                    currentDistance = distanceBetweenStations(station1, station2);

                    if (currentDistance > maximumDistance)
                    {
                        maximumDistance = currentDistance;
                        maxStation1 = station1;
                        maxStation2 = station2;
                    }
                }
            }


            // build route from maxStation1 to maxStation2
            double totalRouteDistance = 0;

            TransportStation currentStation = maxStation1;
            double minDistance = INT_MAX;
            auto deletedStationIter = route.second.begin();
        
            while (currentStation != maxStation2)
            {
                // find the nearest station to the current one
                minDistance = INT_MAX;
                for (auto stationIter = route.second.begin(); stationIter != route.second.end(); stationIter++)
                {

                    if (distanceBetweenStations(currentStation, *stationIter) < minDistance)
                    {
                        minDistance = distanceBetweenStations(currentStation, *stationIter);
                        deletedStationIter = stationIter;
                    }
                }

                // remove the nearest station from the 
                totalRouteDistance += minDistance;
                currentStation = *deletedStationIter;
                route.second.erase(deletedStationIter);
            }

            
            // update maximumRouteLength
            if (totalRouteDistance > maximumRouteLength)
            {
                maximumRouteLength = totalRouteDistance;
                TransportDataItem item = { route.first, maximumRouteLength };
                longestRoutes[vehicle.first] = item;
            }
        }
        

    }    
}

void TransportDataAnalyzer::findStreetWithMaximumStations()
{
    std::map<std::wstring, int> streets;

    // iterate through all stations
    for (pugi::xml_node currentStation : document.child(L"dataset").children(L"transport_station"))
    {
        std::vector<std::wstring> currentStationLocation = splitStringByDelimiters(currentStation.child_value(L"location"), L",");
        for (auto street : currentStationLocation)
        {
            streets[street]++;
        }
    }

    // find street with maximum stations
    std::wstring resultStreet;
    int maximumStations = 0;

    for (auto street : streets)
    {
        if (street.second > maximumStations)
        {
            maximumStations = street.second;
            resultStreet = street.first;
        }
    }

    streetWithMaximumStations = { resultStreet, (double) maximumStations };
}








void TransportDataAnalyzer::outputData() {
    std::wcout << "#1. The route with the maximum number of stations\n";
    for (auto route : routesWithMaximumStations)
    {
        std::wcout << "\t" << route.first << "   \t" << route.second.name << "\t" << route.second.value << " st.\n";
    }
    std::wcout << "\n\n\n";


    std::wcout << "#2. The longest route\n";
    for (auto route : longestRoutes)
    {
        std::wcout << "\t" << route.first << "   \t" << route.second.name << "\t" << route.second.value << " deg\n";
    }
    std::wcout << "\n\n\n";

    std::wcout << "#2. The street with the maximum number of stations\n";
    std::wcout << "\t" << streetWithMaximumStations.name << "\t" << streetWithMaximumStations.value << " st.\n";
}

TransportDataAnalyzer::TransportDataAnalyzer(const wchar_t *fileName)
{
    loadXmlData(fileName);
}

void TransportDataAnalyzer::analyze()
{
    if (!xmlParseResult)
    {
        std::wcout << "Can't load data!" << std::endl;
        return;
    }

    findVehicles();
    findRoutesWithMaximumStations();
    findLongestRoutes();
    findStreetWithMaximumStations();
}

void TransportDataAnalyzer::loadXmlData(const wchar_t* fileName)
{
    this->fileName = (wchar_t *) fileName;
    xmlParseResult = document.load_file(fileName);

    if (!xmlParseResult)
        std::wcout << "File not found!" << std::endl;
}





int main() {
    TransportDataAnalyzer transportDataAnalyzer(L"data.xml");
    transportDataAnalyzer.analyze();
    transportDataAnalyzer.outputData();

    return 0;
}