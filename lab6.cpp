#include <GLFW/glfw3.h>
#include <math.h>
#include "glRapidFunc.h"
#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include "imgui.h"
#include "imgui_impl_glfw_gl2.h"
#include <map>
#include <fstream>
#include <set>
#include <random>

// constants
#define M_PI 3.141592653589793


// rubiks cube class
class RubiksCube
{
    // parameters
    static const int SIZE = 3;
    float BORDER_SIZE = 0.05f;
    float CUBE_SIZE = 0.9f;

    // coordinates
    std::vector<float> vertices[SIZE][SIZE][SIZE];
    std::vector<float> verticesColors[SIZE][SIZE][SIZE];

    // configuration
    char frontFace[SIZE][SIZE] = {
        'G', 'G', 'G',
        'G', 'G', 'G',
        'G', 'G', 'G'
    };
    char backFace[SIZE][SIZE] = {
        'B', 'B', 'B',
        'B', 'B', 'B',
        'B', 'B', 'B'
    };
    char leftFace[SIZE][SIZE] = {
        'R', 'R', 'R',
        'R', 'R', 'R',
        'R', 'R', 'R'
    };
    char rightFace[SIZE][SIZE] = {
        'O', 'O', 'O',
        'O', 'O', 'O',
        'O', 'O', 'O'
    };
    char topFace[SIZE][SIZE] = {
        'Y', 'Y', 'Y',
        'Y', 'Y', 'Y',
        'Y', 'Y', 'Y'
    };
    char bottomFace[SIZE][SIZE] = {
        'W', 'W', 'W',
        'W', 'W', 'W',
        'W', 'W', 'W'
    };



    std::array<float, 3> getColor(char colorCode)
    {
        switch (colorCode)
        {
            case 'Y':
                return { rCOLOR_YELLOW[0], rCOLOR_YELLOW[1], rCOLOR_YELLOW[2] };
            case 'R':
                return { rCOLOR_RED[0], rCOLOR_RED[1], rCOLOR_RED[2] };
            case 'G':
                return { rCOLOR_GREEN[0], rCOLOR_GREEN[1], rCOLOR_GREEN[2] };
            case 'O':
                return { rCOLOR_ORANGE[0], rCOLOR_ORANGE[1], rCOLOR_ORANGE[2] };
            case 'B':
                return { rCOLOR_BLUE[0], rCOLOR_BLUE[1], rCOLOR_BLUE[2] };
            case 'W':
                return { rCOLOR_WHITE[0], rCOLOR_WHITE[1], rCOLOR_WHITE[2] };
        }
    }





    void initialize()
    {
        // clear vertices
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++)
                    vertices[i][j][k].clear();

        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++)
                    verticesColors[i][j][k].clear();


        // initialize vertices
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                for (int z = 0; z < SIZE; z++)
                {
                    float stickers[72] = {
                        // top (y == max)
                        x + BORDER_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE + CUBE_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE + CUBE_SIZE,

                        // bottom (y == min)
                        x + BORDER_SIZE, y + BORDER_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE, y + BORDER_SIZE, z + BORDER_SIZE + CUBE_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE, z + BORDER_SIZE + CUBE_SIZE,

                        // front (x == min)
                        x + BORDER_SIZE, y + BORDER_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE, y + BORDER_SIZE, z + BORDER_SIZE + CUBE_SIZE,
                        x + BORDER_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE + CUBE_SIZE,

                        // back (x == max)
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE, z + BORDER_SIZE + CUBE_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE + CUBE_SIZE,

                        // left (z == min)
                        x + BORDER_SIZE, y + BORDER_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE,

                        // right (z == max)
                        x + BORDER_SIZE, y + BORDER_SIZE, z + BORDER_SIZE + CUBE_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE, z + BORDER_SIZE + CUBE_SIZE,
                        x + BORDER_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE + CUBE_SIZE,
                        x + BORDER_SIZE + CUBE_SIZE, y + BORDER_SIZE + CUBE_SIZE, z + BORDER_SIZE + CUBE_SIZE

                    };

                    // add stickers
                    for (int i = 0; i < 72; i++)
                    {
                        vertices[x][y][z].push_back(stickers[i]);
                    }

                    // colors
                    for (int i = 0; i < 6; i++)
                    {
                        float currentColor[3] = { rCOLOR_BLACK[0], rCOLOR_BLACK[1], rCOLOR_BLACK[2] };

                        if (i == 0 && y == SIZE - 1)
                        {
                            currentColor[0] = getColor(topFace[SIZE - x - 1][z])[0];
                            currentColor[1] = getColor(topFace[SIZE - x - 1][z])[1];
                            currentColor[2] = getColor(topFace[SIZE - x - 1][z])[2];
                        }
                        else if (i == 1 && y == 0)
                        {
                            currentColor[0] = getColor(bottomFace[x][z])[0];
                            currentColor[1] = getColor(bottomFace[x][z])[1];
                            currentColor[2] = getColor(bottomFace[x][z])[2];
                        }
                        else if (i == 2 && x == 0)
                        {
                            currentColor[0] = getColor(frontFace[SIZE - y - 1][z])[0];
                            currentColor[1] = getColor(frontFace[SIZE - y - 1][z])[1];
                            currentColor[2] = getColor(frontFace[SIZE - y - 1][z])[2];
                        }
                        else if (i == 3 && x == SIZE - 1)
                        {
                            currentColor[0] = getColor(backFace[SIZE - y - 1][SIZE - z - 1])[0];
                            currentColor[1] = getColor(backFace[SIZE - y - 1][SIZE - z - 1])[1];
                            currentColor[2] = getColor(backFace[SIZE - y - 1][SIZE - z - 1])[2];
                        }
                        else if (i == 4 && z == 0)
                        {
                            currentColor[0] = getColor(leftFace[SIZE - y - 1][SIZE - x - 1])[0];
                            currentColor[1] = getColor(leftFace[SIZE - y - 1][SIZE - x - 1])[1];
                            currentColor[2] = getColor(leftFace[SIZE - y - 1][SIZE - x - 1])[2];
                        }
                        else if (i == 5 && z == SIZE - 1)
                        {
                            currentColor[0] = getColor(rightFace[SIZE - y - 1][x])[0];
                            currentColor[1] = getColor(rightFace[SIZE - y - 1][x])[1];
                            currentColor[2] = getColor(rightFace[SIZE - y - 1][x])[2];
                        }




                        verticesColors[x][y][z].push_back(currentColor[0]);
                        verticesColors[x][y][z].push_back(currentColor[1]);
                        verticesColors[x][y][z].push_back(currentColor[2]);
                    }



                }
            }
        }
    }




public:
    RubiksCube()
    {
        initialize();
    }

    void loadConfigurationFile(const std::string& fileName)
    {
        std::set<char> colorsSymbols;
        colorsSymbols.insert('Y');
        colorsSymbols.insert('R');
        colorsSymbols.insert('G');
        colorsSymbols.insert('O');
        colorsSymbols.insert('B');
        colorsSymbols.insert('W');

        // read

        std::vector<char> symbols;

        std::ifstream fin(fileName.c_str());
        char symbol;

        int symbolsRead = 0;
        int charCount = 0;
        while (symbolsRead < SIZE * SIZE * 6)
        {
            charCount++;
            fin >> symbol;

            if (colorsSymbols.find(symbol) != colorsSymbols.end())
            {
                symbols.push_back(symbol);
                symbolsRead++;
            }

            if (charCount >= 500)
            {
                std::cout << "Invalid format. Too many chars or too few color symbols!\n";
                return;
            }
           
        }

        // initialize
        int symbolCount = 0;

        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                topFace[i][j] = symbols[symbolCount++];

        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
                leftFace[i][j] = symbols[symbolCount++];
            for (int j = 0; j < SIZE; j++)
                frontFace[i][j] = symbols[symbolCount++];
            for (int j = 0; j < SIZE; j++)
                rightFace[i][j] = symbols[symbolCount++];
            for (int j = 0; j < SIZE; j++)
                backFace[i][j] = symbols[symbolCount++];
        }

        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                bottomFace[i][j] = symbols[symbolCount++];

        initialize();

        std::cout << "The configuration was loaded\n";

    }


    void saveConfigurationFile(const std::string& fileName)
    {
        std::ofstream fout(fileName.c_str());
        for (int i = 0; i < SIZE; i++)
        {
            fout << "   ";
            for (int j = 0; j < SIZE; j++)
                fout << topFace[i][j];
            fout << "\n";
        }

        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
                fout << leftFace[i][j];
            for (int j = 0; j < SIZE; j++)
                fout << frontFace[i][j];
            for (int j = 0; j < SIZE; j++)
                fout << rightFace[i][j];
            for (int j = 0; j < SIZE; j++)
                fout << backFace[i][j];
            fout << "\n";
        }

        for (int i = 0; i < SIZE; i++)
        {
            fout << "   ";
            for (int j = 0; j < SIZE; j++)
                fout << bottomFace[i][j];
            fout << "\n";
        }


        std::cout << "The configuration was saved!";
    }







    std::array<std::array<char, SIZE>, SIZE> getFace(const std::string& faceName)
    {
        std::array<std::array<char, SIZE>, SIZE> arr;
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (faceName == "top")
                    arr[i][j] = topFace[i][j];
                else if (faceName == "bottom")
                    arr[i][j] = bottomFace[i][j];
                else if (faceName == "front")
                    arr[i][j] = frontFace[i][j];
                else if (faceName == "back")
                    arr[i][j] = backFace[i][j];
                else if (faceName == "left")
                    arr[i][j] = leftFace[i][j];
                else if (faceName == "right")
                    arr[i][j] = rightFace[i][j];

            }
        }

        

        return arr;

    }





    void clone(const RubiksCube& cube)
    {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++)
                    vertices[i][j][k] = cube.vertices[i][j][k];

        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++)
                    verticesColors[i][j][k] = cube.verticesColors[i][j][k];


        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                topFace[i][j] = cube.topFace[i][j];
                topFace[i][j] = cube.topFace[i][j];
                topFace[i][j] = cube.topFace[i][j];

                leftFace[i][j] = cube.leftFace[i][j];
                leftFace[i][j] = cube.leftFace[i][j];
                leftFace[i][j] = cube.leftFace[i][j];

                frontFace[i][j] = cube.frontFace[i][j];
                frontFace[i][j] = cube.frontFace[i][j];
                frontFace[i][j] = cube.frontFace[i][j];

                rightFace[i][j] = cube.rightFace[i][j];
                rightFace[i][j] = cube.rightFace[i][j];
                rightFace[i][j] = cube.rightFace[i][j];

                backFace[i][j] = cube.backFace[i][j];
                backFace[i][j] = cube.backFace[i][j];
                backFace[i][j] = cube.backFace[i][j];

                bottomFace[i][j] = cube.bottomFace[i][j];
                bottomFace[i][j] = cube.bottomFace[i][j];
                bottomFace[i][j] = cube.bottomFace[i][j];

            }
        }
    }


    void draw()
    {
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                for (int z = 0; z < SIZE; z++)
                {
                    rDrawVectorWithQuadrangles(vertices[x][y][z], verticesColors[x][y][z]);
                }
            }
        }
        
        
    }


    void outputCurrentState()
    {
        // top face
        for (int i = 0; i < SIZE; i++)
        {
            std::cout << "   ";
            for (int j = 0; j < SIZE; j++)
            {
                std::cout << topFace[i][j];
            }
            std::cout << "\n";
        }

        for (int i = 0; i < SIZE; i++)
        {
            // left face
            for (int j = 0; j < SIZE; j++)
                std::cout << leftFace[i][j];

            // front face
            for (int j = 0; j < SIZE; j++)
                std::cout << frontFace[i][j];

            // right face
            for (int j = 0; j < SIZE; j++)
                std::cout << rightFace[i][j];

            // back face
            for (int j = 0; j < SIZE; j++)
                std::cout << backFace[i][j];

            std::cout << "\n";
        }

        // bottom face
        for (int i = 0; i < SIZE; i++)
        {
            std::cout << "   ";
            for (int j = 0; j < SIZE; j++)
            {
                std::cout << bottomFace[i][j];
            }
            std::cout << "\n";
        }
       
        std::cout << "\n\n\n";

    }



    void turn(std::string query, float angle)
    {

        if (*(query.rbegin()) == '\'') {
            angle = -angle;
            query = query.substr(0, query.size() - 1);
        }

        if (query == "F")
        {
            xTurn(true, false, false, -angle);
        }
        else if (query == "B")
        {
            xTurn(false, false, true, angle);
        }
        else if (query == "L")
        {
            zTurn(true, false, false, -angle);
        }
        else if (query == "R")
        {
            zTurn(false, false, true, angle);
        }
        else if (query == "U")
        {
            yTurn(false, false, true, -angle);
        }
        else if (query == "D")
        {
            yTurn(true, false, false, angle);
        }
        else if (query == "X")
        {
            zTurn(true, true, true, angle);
        }
        else if (query == "Y")
        {
            yTurn(true, true, true, -angle);
        }
        else if (query == "Z")
        {
            xTurn(true, true, true, -angle);
        }


        


    }


    void updateState(std::string query)
    {

        bool reverseFlag = false;

        if (*(query.rbegin()) == '\'') {
            reverseFlag = true;
            query = query.substr(0, query.size() - 1);
        }


        if (query == "F")
        {
            updateX(true, false, false, true ^ reverseFlag);
        }
        else if (query == "B")
        {
            updateX(false, false, true, false ^ reverseFlag);
        }
        else if (query == "L")
        {
            updateZ(true, false, false, true ^ reverseFlag);
        }
        else if (query == "R")
        {
            updateZ(false, false, true, false ^ reverseFlag);
        }
        else if (query == "U")
        {
            updateY(false, false, true, true ^ reverseFlag);
        }
        else if (query == "D")
        {
            updateY(true, false, false, false ^ reverseFlag);
        }
        else if (query == "X")
        {
            updateZ(true, true, true, false ^ reverseFlag);
        }
        else if (query == "Y")
        {
            updateY(true, true, true, true ^ reverseFlag);
        }
        else if (query == "Z")
        {
            updateX(true, true, true, true ^ reverseFlag);
        }




    }

    void updateX(bool firstLayer, bool secondLayer, bool thirdLayer, bool isPositiveAngle)
    {
        for (int x = 0; x < SIZE; x++)
        {
            if (x == 0 && firstLayer || x == 1 && secondLayer || x == 2 && thirdLayer)
            {
                std::vector<float> verticesBuffer[SIZE][SIZE];
                std::vector<float> verticesColorsBuffer[SIZE][SIZE];



                for (int z = 0; z < SIZE; z++)
                {
                    for (int y = 0; y < SIZE; y++)
                    {
                        verticesBuffer[y][z] = vertices[x][y][z];
                        verticesColorsBuffer[y][z] = verticesColors[x][y][z];

                    }
                }

                for (int z = 0; z < SIZE; z++)
                {
                    for (int y = 0; y < SIZE; y++)
                    {
                        if (isPositiveAngle)
                        {
                            vertices[x][y][z] = verticesBuffer[z][SIZE - y - 1];
                            verticesColors[x][y][z] = verticesColorsBuffer[z][SIZE - y - 1];

                            // change char faces configuration
                                                        // change char
                            char charFaceBuffer[3] = { topFace[SIZE - x - 1][0], topFace[SIZE - x - 1][1], topFace[SIZE - x - 1][2] };       // clockwise
                            for (int i = 0; i < SIZE; i++)
                            {
                                topFace[SIZE - x - 1][i] = leftFace[SIZE - i - 1][SIZE - x - 1];
                                leftFace[SIZE - i - 1][SIZE - x - 1] = bottomFace[x][SIZE - i - 1];
                                bottomFace[x][SIZE - i - 1] = rightFace[i][x];
                                rightFace[i][x] = charFaceBuffer[i];

                            }

                            if (x == 0)         // front
                            {
                                char charFaceBuffer2[2] = { frontFace[0][0], frontFace[0][1] };
                                frontFace[0][1] = frontFace[1][0];  frontFace[0][0] = frontFace[2][0];
                                frontFace[1][0] = frontFace[2][1];  frontFace[2][0] = frontFace[2][2];
                                frontFace[2][1] = frontFace[1][2];  frontFace[2][2] = frontFace[0][2];
                                frontFace[1][2] = charFaceBuffer2[1];  frontFace[0][2] = charFaceBuffer2[0];



                            }
                            else if (x == 2)    // back
                            {
                                char charFaceBuffer2[2] = { backFace[0][0], backFace[0][1] };
                                backFace[0][1] = backFace[1][2];  backFace[0][0] = backFace[0][2];
                                backFace[1][2] = backFace[2][1];  backFace[0][2] = backFace[2][2];
                                backFace[2][1] = backFace[1][0];  backFace[2][2] = backFace[2][0];
                                backFace[1][0] = charFaceBuffer2[1];  backFace[2][0] = charFaceBuffer2[0];
                            }

                            
                        }
                        else
                        {
                            vertices[x][y][z] = verticesBuffer[SIZE - z - 1][y];
                            verticesColors[x][y][z] = verticesColorsBuffer[SIZE - z - 1][y];

                            // change char faces configuration
                            char charFaceBuffer[3] = { topFace[SIZE - x - 1][0], topFace[SIZE - x - 1][1], topFace[SIZE - x - 1][2] };       // counter-clockwise
                            for (int i = 0; i < SIZE; i++)
                            {
                                topFace[SIZE - x - 1][i] = rightFace[i][x];
                                rightFace[i][x] = bottomFace[x][SIZE - i - 1];
                                bottomFace[x][SIZE - i - 1] = leftFace[SIZE - i - 1][SIZE - x - 1];
                                leftFace[SIZE - i - 1][SIZE - x - 1] = charFaceBuffer[i];
                            }

                            if (x == 0)         // front
                            {
                                char charFaceBuffer2[2] = { frontFace[0][0], frontFace[0][1] };
                                frontFace[0][1] = frontFace[1][2];  frontFace[0][0] = frontFace[0][2];
                                frontFace[1][2] = frontFace[2][1];  frontFace[0][2] = frontFace[2][2];
                                frontFace[2][1] = frontFace[1][0];  frontFace[2][2] = frontFace[2][0];
                                frontFace[1][0] = charFaceBuffer2[1];  frontFace[2][0] = charFaceBuffer2[0];

                            }
                            else if (x == 2)    // back
                            {



                                char charFaceBuffer2[2] = { backFace[0][0], backFace[0][1] };
                                backFace[0][1] = backFace[1][0];  backFace[0][0] = backFace[2][0];
                                backFace[1][0] = backFace[2][1];  backFace[2][0] = backFace[2][2];
                                backFace[2][1] = backFace[1][2];  backFace[2][2] = backFace[0][2];
                                backFace[1][2] = charFaceBuffer2[1];  backFace[0][2] = charFaceBuffer2[0];

                            }
                        }
                    }
                }

            }
        }
    }

    void updateY(bool firstLayer, bool secondLayer, bool thirdLayer, bool isPositiveAngle)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (y == 0 && firstLayer || y == 1 && secondLayer || y == 2 && thirdLayer)
            {
                std::vector<float> verticesBuffer[SIZE][SIZE];
                std::vector<float> verticesColorsBuffer[SIZE][SIZE];



                for (int z = 0; z < SIZE; z++)
                {
                    for (int x = 0; x < SIZE; x++)
                    {
                        verticesBuffer[x][z] = vertices[x][y][z];
                        verticesColorsBuffer[x][z] = verticesColors[x][y][z];
                        
                    }
                }

                for (int z = 0; z < SIZE; z++)
                {
                    for (int x = 0; x < SIZE; x++)
                    {
                        if (isPositiveAngle)
                        {
                            vertices[x][y][z] = verticesBuffer[z][SIZE - x - 1];
                            verticesColors[x][y][z] = verticesColorsBuffer[z][SIZE - x - 1];

                            // change char faces configuration
                            char charFaceBuffer[3] = { leftFace[SIZE - y - 1][0], leftFace[SIZE - y - 1][1], leftFace[SIZE - y - 1][2] };
                            for (int j = 0; j < SIZE; j++)
                            {
                                leftFace[SIZE - y - 1][j] = frontFace[SIZE - y - 1][j];
                                frontFace[SIZE - y - 1][j] = rightFace[SIZE - y - 1][j];
                                rightFace[SIZE - y - 1][j] = backFace[SIZE - y - 1][j];
                                backFace[SIZE - y - 1][j] = charFaceBuffer[j];
                            }

                            if (y == 2)
                            {
                                char charFaceBuffer2[2] = { topFace[0][0], topFace[0][1] };
                                topFace[0][1] = topFace[1][0];  topFace[0][0] = topFace[2][0];
                                topFace[1][0] = topFace[2][1];  topFace[2][0] = topFace[2][2];
                                topFace[2][1] = topFace[1][2];  topFace[2][2] = topFace[0][2];
                                topFace[1][2] = charFaceBuffer2[1];  topFace[0][2] = charFaceBuffer2[0];

                            }
                            else if (y == 0)
                            {
                                char charFaceBuffer2[2] = { bottomFace[0][0], bottomFace[0][1] };
                                bottomFace[0][1] = bottomFace[1][2];  bottomFace[0][0] = bottomFace[0][2];
                                bottomFace[1][2] = bottomFace[2][1];  bottomFace[0][2] = bottomFace[2][2];
                                bottomFace[2][1] = bottomFace[1][0];  bottomFace[2][2] = bottomFace[2][0];
                                bottomFace[1][0] = charFaceBuffer2[1];  bottomFace[2][0] = charFaceBuffer2[0];
                            }
                        }
                        else
                        {
                            vertices[x][y][z] = verticesBuffer[SIZE - z - 1][x];
                            verticesColors[x][y][z] = verticesColorsBuffer[SIZE - z - 1][x];

                            // change char faces configuration
                            char charFaceBuffer[3] = { backFace[SIZE - y - 1][0], backFace[SIZE - y - 1][1], backFace[SIZE - y - 1][2] };
                            for (int j = 0; j < SIZE; j++)
                            {
                                backFace[SIZE - y - 1][j] = rightFace[SIZE - y - 1][j];
                                rightFace[SIZE - y - 1][j] = frontFace[SIZE - y - 1][j];
                                frontFace[SIZE - y - 1][j] = leftFace[SIZE - y - 1][j];
                                leftFace[SIZE - y - 1][j] = charFaceBuffer[j];
                            }

                            if (y == 2)
                            {
                                char charFaceBuffer2[2] = { topFace[0][0], topFace[0][1] };
                                topFace[0][1] = topFace[1][2];  topFace[0][0] = topFace[0][2];
                                topFace[1][2] = topFace[2][1];  topFace[0][2] = topFace[2][2];
                                topFace[2][1] = topFace[1][0];  topFace[2][2] = topFace[2][0];
                                topFace[1][0] = charFaceBuffer2[1];  topFace[2][0] = charFaceBuffer2[0];

                            }
                            else if (y == 0)
                            {
                                char charFaceBuffer2[2] = { bottomFace[0][0], bottomFace[0][1] };
                                bottomFace[0][1] = bottomFace[1][0];  bottomFace[0][0] = bottomFace[2][0];
                                bottomFace[1][0] = bottomFace[2][1];  bottomFace[2][0] = bottomFace[2][2];
                                bottomFace[2][1] = bottomFace[1][2];  bottomFace[2][2] = bottomFace[0][2];
                                bottomFace[1][2] = charFaceBuffer2[1];  bottomFace[0][2] = charFaceBuffer2[0];
                            }
                        }
                    }
                }

            }
        }
    }

    void updateZ(bool firstLayer, bool secondLayer, bool thirdLayer, bool isPositiveAngle)
    {
        for (int z = 0; z < SIZE; z++)
        {
            if (z == 0 && firstLayer || z == 1 && secondLayer || z == 2 && thirdLayer)
            {
                std::vector<float> verticesBuffer[SIZE][SIZE];
                std::vector<float> verticesColorsBuffer[SIZE][SIZE];



                for (int y = 0; y < SIZE; y++)
                {
                    for (int x = 0; x < SIZE; x++)
                    {
                        verticesBuffer[x][y] = vertices[x][y][z];
                        verticesColorsBuffer[x][y] = verticesColors[x][y][z];

                    }
                }

                for (int y = 0; y < SIZE; y++)
                {
                    for (int x = 0; x < SIZE; x++)
                    {
                        if (isPositiveAngle)
                        {
                            vertices[x][y][z] = verticesBuffer[y][SIZE - x - 1];
                            verticesColors[x][y][z] = verticesColorsBuffer[y][SIZE - x - 1];

                            // change char faces configuration
                            char charFaceBuffer[3] = { backFace[2][SIZE - z - 1], backFace[1][SIZE - z - 1], backFace[0][SIZE - z - 1] };
                            for (int i = 0; i < SIZE; i++)
                            {
                                backFace[SIZE - i - 1][SIZE - z - 1] = bottomFace[i][z];
                                bottomFace[i][z] = frontFace[i][z];
                                frontFace[i][z] = topFace[i][z];
                                topFace[i][z] = charFaceBuffer[i];
                            }

                            if (z == 0)     // left
                            {
                                char charFaceBuffer2[2] = { leftFace[0][0], leftFace[0][1] };
                                leftFace[0][1] = leftFace[1][0];  leftFace[0][0] = leftFace[2][0];
                                leftFace[1][0] = leftFace[2][1];  leftFace[2][0] = leftFace[2][2];
                                leftFace[2][1] = leftFace[1][2];  leftFace[2][2] = leftFace[0][2];
                                leftFace[1][2] = charFaceBuffer2[1];  leftFace[0][2] = charFaceBuffer2[0];
                            }
                            else if (z == 2)    // right
                            {
                                char charFaceBuffer2[2] = { rightFace[0][0], rightFace[0][1] };
                                rightFace[0][1] = rightFace[1][2];  rightFace[0][0] = rightFace[0][2];
                                rightFace[1][2] = rightFace[2][1];  rightFace[0][2] = rightFace[2][2];
                                rightFace[2][1] = rightFace[1][0];  rightFace[2][2] = rightFace[2][0];
                                rightFace[1][0] = charFaceBuffer2[1];  rightFace[2][0] = charFaceBuffer2[0];
                            }

                        }
                        else
                        {
                            vertices[x][y][z] = verticesBuffer[SIZE - y - 1][x];
                            verticesColors[x][y][z] = verticesColorsBuffer[SIZE - y - 1][x];

                            // change char faces configuration
                            char charFaceBuffer[3] = { backFace[2][SIZE - z - 1], backFace[1][SIZE - z - 1], backFace[0][SIZE - z - 1] };
                            for (int i = 0; i < SIZE; i++)
                            {
                                backFace[SIZE - i - 1][SIZE - z - 1] = topFace[i][z];
                                topFace[i][z] = frontFace[i][z];
                                frontFace[i][z] = bottomFace[i][z];
                                bottomFace[i][z] = charFaceBuffer[i];
                            }

                            if (z == 0)         // left
                            {
                                char charFaceBuffer2[2] = { leftFace[0][0], leftFace[0][1] };
                                leftFace[0][1] = leftFace[1][2];  leftFace[0][0] = leftFace[0][2];
                                leftFace[1][2] = leftFace[2][1];  leftFace[0][2] = leftFace[2][2];
                                leftFace[2][1] = leftFace[1][0];  leftFace[2][2] = leftFace[2][0];
                                leftFace[1][0] = charFaceBuffer2[1];  leftFace[2][0] = charFaceBuffer2[0];
                            }
                            else if (z == 2)    // right
                            {
                                char charFaceBuffer2[2] = { rightFace[0][0], rightFace[0][1] };
                                rightFace[0][1] = rightFace[1][0];  rightFace[0][0] = rightFace[2][0];
                                rightFace[1][0] = rightFace[2][1];  rightFace[2][0] = rightFace[2][2];
                                rightFace[2][1] = rightFace[1][2];  rightFace[2][2] = rightFace[0][2];
                                rightFace[1][2] = charFaceBuffer2[1];  rightFace[0][2] = charFaceBuffer2[0];


                            }
                        }
                    }
                }

            }
        }
    }



    void xTurn(bool firstLayer, bool secondLayer, bool thirdLayer, float angle)
    {
        angle = angle / 180 * M_PI;
        float centerY = 1.5f;
        float centerZ = 1.5f;

        for (int y = 0; y < SIZE; y++)
        {
            for (int z = 0; z < SIZE; z++)
            {
                for (int x = 0; x < SIZE; x++)
                {
                    if (x == 0 && firstLayer || x == 1 && secondLayer || x == 2 && thirdLayer)
                    {
                        int coordinatesSize = vertices[x][y][z].size();
                        float y1, z1, y2, z2, y3, z3, y4, z4;
                        for (int i = 0; i < coordinatesSize; i += 3)
                        {
                            y1 = vertices[x][y][z][i + 1];
                            z1 = vertices[x][y][z][i + 2];

                            y2 = y1 - centerY;
                            z2 = z1 - centerZ;

                            y3 = y2 * cos(angle) + z2 * sin(angle);
                            z3 = -y2 * sin(angle) + z2 * cos(angle);

                            y4 = y3 + centerY;
                            z4 = z3 + centerZ;

                            vertices[x][y][z][i + 1] = y4;
                            vertices[x][y][z][i + 2] = z4;
                        }
                    }

                }


            }
        }
    }


    void yTurn(bool firstLayer, bool secondLayer, bool thirdLayer, float angle)
    {
        angle = angle / 180 * M_PI;
        float centerX = 1.5f;
        float centerZ = 1.5f;
        
        for (int x = 0; x < SIZE; x++)
        {
            for (int z = 0; z < SIZE; z++)
            {
                for (int y = 0; y < SIZE; y++)
                {
                    if (y == 0 && firstLayer || y == 1 && secondLayer || y == 2 && thirdLayer)
                    {
                        int coordinatesSize = vertices[x][y][z].size();
                        float x1, z1, x2, z2, x3, z3, x4, z4;
                        for (int i = 0; i < coordinatesSize; i += 3)
                        {
                            x1 = vertices[x][y][z][i];
                            z1 = vertices[x][y][z][i + 2];

                            x2 = x1 - centerX;
                            z2 = z1 - centerZ;

                            x3 = x2 * cos(angle) + z2 * sin(angle);
                            z3 = -x2 * sin(angle) + z2 * cos(angle);

                            x4 = x3 + centerX;
                            z4 = z3 + centerZ;

                            vertices[x][y][z][i] = x4;
                            vertices[x][y][z][i + 2] = z4;
                        }
                    }

                }
 

            }
        }


    }


    void zTurn(bool firstLayer, bool secondLayer, bool thirdLayer, float angle)
    {
        angle = angle / 180 * M_PI;
        float centerX = 1.5f;
        float centerY = 1.5f;

        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                for (int z = 0; z < SIZE; z++)
                {
                    if (z == 0 && firstLayer || z == 1 && secondLayer || z == 2 && thirdLayer)
                    {
                        int coordinatesSize = vertices[x][y][z].size();
                        float x1, y1, x2, y2, x3, y3, x4, y4;
                        for (int i = 0; i < coordinatesSize; i += 3)
                        {
                            x1 = vertices[x][y][z][i];
                            y1 = vertices[x][y][z][i + 1];

                            x2 = x1 - centerX;
                            y2 = y1 - centerY;

                            x3 = x2 * cos(angle) + y2 * sin(angle);
                            y3 = -x2 * sin(angle) + y2 * cos(angle);

                            x4 = x3 + centerX;
                            y4 = y3 + centerY;

                            vertices[x][y][z][i] = x4;
                            vertices[x][y][z][i + 1] = y4;
                        }
                    }

                }


            }
        }
    }





};

// rubiks cube class end


// rubiks cube solver class
class RubiksCubeSolver
{
    static const int SIZE = 3;

    RubiksCube cube;
    std::vector<std::string> turnSequence;

    char faceBuffer[SIZE][SIZE];





    void copyFaceToBuffer(const std::array<std::array<char, SIZE>, SIZE>& face)
    {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                faceBuffer[i][j] = face[i][j];
    }

    int colorCodeToInt(char colorCode)
    {
        if (colorCode == 'W')
            return 0;
        if (colorCode == 'B')
            return 1;
        if (colorCode == 'R')
            return 2;
        if (colorCode == 'O')
            return 3;
        if (colorCode == 'G')
            return 4;
        if (colorCode == 'Y')
            return 5;
    }



    void addTurn(const std::string& turn, RubiksCube& cube)
    {
        turnSequence.push_back(turn);
        cube.updateState(turn);
        cube.turn(turn, 90);
        //cube.outputCurrentState();
    }

public:


    

    bool firstCheck(RubiksCube& cube)
    {
        
        // check stickers count

        char counts[256] = { 0 };

        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                counts[cube.getFace("top")[i][j]]++;    counts[cube.getFace("bottom")[i][j]]++;     counts[cube.getFace("left")[i][j]]++;
                counts[cube.getFace("right")[i][j]]++;  counts[cube.getFace("front")[i][j]]++;      counts[cube.getFace("back")[i][j]]++;
            }
        }

        bool res = (counts['G'] == SIZE * SIZE && counts['B'] == SIZE * SIZE && counts['R'] == SIZE * SIZE
            && counts['O'] == SIZE * SIZE && counts['W'] == SIZE * SIZE && counts['Y'] == SIZE * SIZE);

        if (!res)
            return res;

        // check central stickers count
        res = res && (colorCodeToInt(cube.getFace("front")[1][1]) + colorCodeToInt(cube.getFace("back")[1][1]) == 5)
            && (colorCodeToInt(cube.getFace("left")[1][1]) + colorCodeToInt(cube.getFace("right")[1][1]) == 5)
            && (colorCodeToInt(cube.getFace("top")[1][1]) + colorCodeToInt(cube.getFace("bottom")[1][1]) == 5);

        // check central stickers orientation
        char possibleOptions[24][3] = {
            'W', 'B', 'O',      'W', 'R', 'B',      'W', 'G', 'R',      'W', 'O', 'G',
            'Y', 'B', 'R',      'Y', 'O', 'B',      'Y', 'G', 'O',      'Y', 'R', 'G',
            'G', 'W', 'O',      'G', 'R', 'W',      'G', 'Y', 'R',      'G', 'O', 'Y',
            'O', 'B', 'Y',      'O', 'W', 'B',      'O', 'G', 'W',      'O', 'Y', 'G',
            'R', 'B', 'W',      'R', 'Y', 'B',      'R', 'G', 'Y',      'R', 'W', 'G',
            'B', 'W', 'R',      'B', 'O', 'W',      'B', 'Y', 'O',      'B', 'R', 'Y'
        };

        res = false;
        for (int i = 0; i < 24; i++)
            res = res || cube.getFace("front")[1][1] == possibleOptions[i][0] && cube.getFace("top")[1][1] == possibleOptions[i][1] && cube.getFace("left")[1][1] == possibleOptions[i][2];

        return res;
        
    }


    void solveCross(RubiksCube& cube)
    {

        bool edgesSolved[6] = { false };
        int edgesSolvedCount = 0;
        char bottomColor = cube.getFace("bottom")[1][1];
        char topColor = cube.getFace("top")[1][1];
        edgesSolved[colorCodeToInt(bottomColor)] = true;
        edgesSolved[colorCodeToInt(topColor)] = true;


        // while all 4 edges are not solved
        while (edgesSolvedCount < 4)
        {
            
            // find edge in front
            bool edgeFound = false;
            char edgeLocation = ' ';

            while (!edgeFound)
            {

                copyFaceToBuffer(cube.getFace("front"));
                // check every face for edge
                if (faceBuffer[2][1] == faceBuffer[1][1] && cube.getFace("bottom")[0][1] == bottomColor && edgesSolved[colorCodeToInt(faceBuffer[1][1])] == false) // edge solved case
                {

                    edgeFound = true;
                    edgesSolved[colorCodeToInt(faceBuffer[1][1])] = true;
                    edgesSolvedCount++;
                    break;
                }
                else if (faceBuffer[0][1] == bottomColor || cube.getFace("top")[2][1] == bottomColor) // top location case
                {


                    edgeFound = true;
                }
                else if (faceBuffer[1][0] == bottomColor || cube.getFace("left")[1][2] == bottomColor) // left location case
                {

                    edgeFound = true;

                    addTurn("L'", cube);
                    addTurn("U'", cube);
                    addTurn("L", cube);

                }
                else if (faceBuffer[1][2] == bottomColor || cube.getFace("right")[1][0] == bottomColor) // right location case
                {

                    edgeFound = true;
                    addTurn("R", cube);
                    addTurn("U", cube);
                    addTurn("R'", cube);
                }
                else if (faceBuffer[2][1] == bottomColor || cube.getFace("bottom")[0][1] == bottomColor && faceBuffer[2][1] != faceBuffer[1][1]) // bottom location case ( edge color != center color)
                {
                    edgeFound = true;
                    addTurn("F", cube);
                    addTurn("F", cube);
                }


                // y-turn
                if (!edgeFound)
                {
                    addTurn("Y'", cube);
                }
                else
                {
                    // move to the correct center
                    while (cube.getFace("front")[0][1] != cube.getFace("front")[1][1] && cube.getFace("top")[2][1] != cube.getFace("front")[1][1])
                    {
                        addTurn("U", cube);
                        addTurn("Y'", cube);
                    }

                    // place edge
                    if (cube.getFace("front")[0][1] == cube.getFace("front")[1][1] && cube.getFace("top")[2][1] == bottomColor)
                    {

                        addTurn("F", cube);
                        addTurn("F", cube);
                    }
                    else
                    {
                        addTurn("U'", cube);
                        addTurn("R'", cube);
                        addTurn("F", cube);
                        addTurn("R", cube);
                    }

                    if (edgesSolved[colorCodeToInt(cube.getFace("front")[1][1])] == false)
                    {
                        edgesSolved[colorCodeToInt(cube.getFace("front")[1][1])] = true;
                        edgesSolvedCount++;
                    }
                }


            }

            // at this point edge is moved at the top

            // y-turn


        }

    }


    void solveFirstLayerCorners(RubiksCube& cube)
    {
        char bottomColor = cube.getFace("bottom")[1][1];
        char topColor = cube.getFace("top")[1][1];
  

        // check if 1st layer is solved
        bool solved = true;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                solved = solved && (cube.getFace("bottom")[i][j] == bottomColor);
        for (int j = 0; j < SIZE; j++)
        {
            solved = solved && (cube.getFace("left")[2][j] == cube.getFace("left")[1][1]) && (cube.getFace("front")[2][j] == cube.getFace("front")[1][1])
                && (cube.getFace("right")[2][j] == cube.getFace("right")[1][1]) && (cube.getFace("back")[2][j] == cube.getFace("back")[1][1]);
        }

        
        bool bottomRightCornerSolvedFlag = false;
        bool elseCasesFlag = false;

        // while not solved
        while (!solved)
        {
            bottomRightCornerSolvedFlag = false;
            elseCasesFlag = false;

            if (cube.getFace("front")[0][2] == bottomColor || cube.getFace("top")[2][2] == bottomColor || cube.getFace("right")[0][0] == bottomColor) // top-right corner case
            {
                int ttt = 6;
            }
            else if (cube.getFace("front")[2][2] == cube.getFace("front")[2][1] && cube.getFace("right")[2][0] == cube.getFace("right")[2][1] && cube.getFace("bottom")[0][2] == bottomColor) // bottom-right corner solved case
            {

                addTurn("Y'", cube);

                bottomRightCornerSolvedFlag = true;
                continue;

                
            }
            else if (cube.getFace("front")[2][2] == bottomColor || cube.getFace("right")[2][0] == bottomColor || cube.getFace("bottom")[0][2] == bottomColor) // bottom-right corner unsolved case
            {
                

                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                addTurn("U'", cube);
            }
            else
            {

                addTurn("Y'", cube);
                elseCasesFlag = true;
                continue;
                
            }




                // find the place for the corner
                while (!(cube.getFace("front")[0][2] == cube.getFace("front")[1][1] && cube.getFace("top")[2][2] == cube.getFace("right")[1][1]
                    || cube.getFace("right")[0][0] == cube.getFace("right")[1][1] && cube.getFace("top")[2][2] == cube.getFace("front")[1][1]
                    || cube.getFace("front")[0][2] == cube.getFace("right")[1][1] && cube.getFace("right")[0][0] == cube.getFace("front")[1][1]))
                {
                    addTurn("Y'", cube);
                    addTurn("U", cube);


                }

                // place the corner
                if (cube.getFace("front")[0][2] == cube.getFace("front")[1][1] && cube.getFace("top")[2][2] == cube.getFace("right")[1][1]) // bottom color on the right face
                {
                    addTurn("R", cube);
                    addTurn("U", cube);
                    addTurn("R'", cube);
                    addTurn("U'", cube);


                }
                else if (cube.getFace("right")[0][0] == cube.getFace("right")[1][1] && cube.getFace("top")[2][2] == cube.getFace("front")[1][1]) // bottom color on the front face
                {
                    addTurn("F'", cube);
                    addTurn("U'", cube);
                    addTurn("F", cube);
                }

                else // bottom color on the top face
                {
                    addTurn("R", cube);
                    addTurn("U", cube);
                    addTurn("U", cube);
                    addTurn("R'", cube);
                    addTurn("U'", cube);
                    addTurn("R", cube);
                    addTurn("U", cube);
                    addTurn("R'", cube);
                }

                addTurn("Y'", cube);
                addTurn("U", cube);


            

            // update solved
            solved = true;
            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++) 
                    solved = solved && (cube.getFace("bottom")[i][j] == bottomColor);
           


            for (int j = 0; j < SIZE; j++)
            {
                solved = solved && (cube.getFace("left")[2][j] == cube.getFace("left")[1][1]) && (cube.getFace("front")[2][j] == cube.getFace("front")[1][1])
                    && (cube.getFace("right")[2][j] == cube.getFace("right")[1][1]) && (cube.getFace("back")[2][j] == cube.getFace("back")[1][1]);
            }

        }



        
    }


    void solveSecondLayer(RubiksCube& cube)
    {
        char bottomColor = cube.getFace("bottom")[1][1];
        char topColor = cube.getFace("top")[1][1];

        bool solved;
        solved = true && (cube.getFace("left")[1][0] == cube.getFace("left")[1][1]) && (cube.getFace("left")[1][1] == cube.getFace("left")[1][2])
            && (cube.getFace("front")[1][0] == cube.getFace("front")[1][1]) && (cube.getFace("front")[1][1] == cube.getFace("front")[1][2])
            && (cube.getFace("right")[1][0] == cube.getFace("right")[1][1]) && (cube.getFace("right")[1][1] == cube.getFace("right")[1][2])
            && (cube.getFace("back")[1][0] == cube.getFace("back")[1][1]) && (cube.getFace("back")[1][1] == cube.getFace("back")[1][2]);

        while (!solved)
        {
            if (cube.getFace("front")[0][1] != topColor && cube.getFace("top")[2][1] != topColor) // front-top edge case
            {

            }
            else if (cube.getFace("front")[1][0] != topColor && cube.getFace("left")[1][2] != topColor && !(cube.getFace("front")[1][0] == cube.getFace("front")[1][1] && cube.getFace("left")[1][2] == cube.getFace("left")[1][1])) // front-left mismatching case
            {
                addTurn("U'", cube);
                addTurn("L'", cube);
                addTurn("U", cube);
                addTurn("L", cube);
                addTurn("U", cube);
                addTurn("F", cube);
                addTurn("U'", cube);
                addTurn("F'", cube);
                addTurn("U", cube);
                addTurn("U", cube);

            }
            else if (cube.getFace("front")[1][2] != topColor && cube.getFace("right")[1][0] != topColor && !(cube.getFace("front")[1][2] == cube.getFace("front")[1][1] && cube.getFace("right")[1][0] == cube.getFace("right")[1][1])) // front-right mismatching case
            {
                addTurn("U", cube);
                addTurn("R", cube);
                addTurn("U'", cube);
                addTurn("R'", cube);
                addTurn("U'", cube);
                addTurn("F'", cube);
                addTurn("U", cube);
                addTurn("F", cube);
                addTurn("U", cube);
                addTurn("U", cube);
            }
            else // else case
            {
                addTurn("Y'", cube);
                continue;
            }



            // find the correct place to insert
            while (cube.getFace("front")[0][1] != cube.getFace("front")[1][1])
            {
                addTurn("Y'", cube);
                addTurn("U", cube);
            }

            // place the edge
            if (cube.getFace("top")[2][1] == cube.getFace("left")[1][1]) // insert to the left
            {
                addTurn("U'", cube);
                addTurn("L'", cube);
                addTurn("U", cube);
                addTurn("L", cube);
                addTurn("U", cube);
                addTurn("F", cube);
                addTurn("U'", cube);
                addTurn("F'", cube);
            }
            else // insert to the right
            {
                addTurn("U", cube);
                addTurn("R", cube);
                addTurn("U'", cube);
                addTurn("R'", cube);
                addTurn("U'", cube);
                addTurn("F'", cube);
                addTurn("U", cube);
                addTurn("F", cube);
            }

            // check if 2nd layer is solved
            solved = true && (cube.getFace("left")[1][0] == cube.getFace("left")[1][1]) && (cube.getFace("left")[1][1] == cube.getFace("left")[1][2])
                && (cube.getFace("front")[1][0] == cube.getFace("front")[1][1]) && (cube.getFace("front")[1][1] == cube.getFace("front")[1][2])
                && (cube.getFace("right")[1][0] == cube.getFace("right")[1][1]) && (cube.getFace("right")[1][1] == cube.getFace("right")[1][2])
                && (cube.getFace("back")[1][0] == cube.getFace("back")[1][1]) && (cube.getFace("back")[1][1] == cube.getFace("back")[1][2]);

        }
    }


    void solveTopCross(RubiksCube& cube)
    {
        char topColor = cube.getFace("top")[1][1];
        
        
        if (cube.getFace("top")[0][1] == topColor && cube.getFace("top")[1][0] == topColor && cube.getFace("top")[1][2] == topColor && cube.getFace("top")[2][1] == topColor) // already solved case
            return;

        if (cube.getFace("top")[2][1] == topColor)
            addTurn("U", cube);

        if (cube.getFace("top")[1][0] == topColor && cube.getFace("top")[1][2] == topColor) // strip case
        {
            addTurn("F", cube);
            addTurn("R", cube);
            addTurn("U", cube);
            addTurn("R'", cube);
            addTurn("U'", cube);
            addTurn("F'", cube);
            return;
        }

        // corner-shaped case
        int tInd = 0;
        for (int i = 0; i < 4; i++)
        {
            if (cube.getFace("top")[0][1] == topColor && cube.getFace("top")[1][0] == topColor)
                break;
            addTurn("U", cube);
            tInd++;
        }

        if (tInd < 4)
        {
            addTurn("F", cube);
            addTurn("U", cube);
            addTurn("R", cube);
            addTurn("U'", cube);
            addTurn("R'", cube);
            addTurn("F'", cube);
        }
        else
        {
            addTurn("F", cube);
            addTurn("R", cube);
            addTurn("U", cube);
            addTurn("R'", cube);
            addTurn("U'", cube);
            addTurn("F'", cube);
            addTurn("U", cube);
            addTurn("U", cube);
            addTurn("F", cube);
            addTurn("U", cube);
            addTurn("R", cube);
            addTurn("U'", cube);
            addTurn("R'", cube);
            addTurn("F'", cube);
        }
    }


    bool solveTopFace(RubiksCube& cube)
    {
        char topColor = cube.getFace("top")[1][1];
        

        bool alreadySolvedCase = true;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                alreadySolvedCase = alreadySolvedCase && (cube.getFace("top")[i][j] == topColor);

        if (alreadySolvedCase)
            return true;


        for (int i = 0; i < 4; i++)
        {
            copyFaceToBuffer(cube.getFace("top"));

            if (faceBuffer[2][0] == topColor && cube.getFace("back")[0][2] == topColor && cube.getFace("right")[0][2] == topColor && cube.getFace("front")[0][2] == topColor) // shape #1
            {
                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                addTurn("U", cube);
                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                return true;
            }
            if (faceBuffer[2][2] == topColor && cube.getFace("front")[0][0] == topColor && cube.getFace("left")[0][0] == topColor && cube.getFace("back")[0][0] == topColor) // shape #2
            {
                addTurn("L'", cube);
                addTurn("U'", cube);
                addTurn("L", cube);
                addTurn("U'", cube);
                addTurn("L'", cube);
                addTurn("U", cube);
                addTurn("U", cube);
                addTurn("L", cube);
                return true;
            }
            if (cube.getFace("left")[0][0] == topColor && cube.getFace("left")[0][2] == topColor && cube.getFace("right")[0][0] == topColor && cube.getFace("right")[0][2] == topColor) // shape #3
            {
                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                addTurn("U", cube);
                addTurn("R", cube);
                addTurn("U'", cube);
                addTurn("R'", cube);
                addTurn("U", cube);
                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                return true;
            }
            if (cube.getFace("back")[0][2] == topColor && cube.getFace("front")[0][0] == topColor && cube.getFace("right")[0][0] == topColor && cube.getFace("right")[0][2] == topColor) // shape #4
            {
                addTurn("L", cube);
                addTurn("U'", cube);
                addTurn("R'", cube);
                addTurn("U", cube);
                addTurn("L'", cube);
                addTurn("U", cube);
                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                addTurn("U", cube);
                addTurn("R", cube);
                return true;
            }
            if (faceBuffer[0][0] == topColor && faceBuffer[2][0] == topColor && cube.getFace("back")[0][0] == topColor && cube.getFace("front")[0][2] == topColor) // shape #5
            {
                addTurn("R'", cube);
                addTurn("F'", cube);
                addTurn("L", cube);
                addTurn("F", cube);
                addTurn("R", cube);
                addTurn("F'", cube);
                addTurn("L'", cube);
                addTurn("F", cube);
                return true;
            }
            if (faceBuffer[0][0] == topColor && faceBuffer[2][2] == topColor && cube.getFace("back")[0][0] == topColor && cube.getFace("left")[0][2] == topColor) // shape #6
            {
                addTurn("R'", cube);
                addTurn("F'", cube);
                addTurn("L'", cube);
                addTurn("F", cube);
                addTurn("R", cube);
                addTurn("F'", cube);
                addTurn("L", cube);
                addTurn("F", cube);
                return true;
            }
            if (faceBuffer[0][0] == topColor && faceBuffer[0][2] == topColor && cube.getFace("front")[0][0] == topColor && cube.getFace("front")[0][2] == topColor) // shape #7
            {
                addTurn("R", cube);
                addTurn("R", cube);
                addTurn("D", cube);
                addTurn("R'", cube);
                addTurn("U", cube);
                addTurn("U", cube);
                addTurn("R", cube);
                addTurn("D'", cube);
                addTurn("R'", cube);
                addTurn("U", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                return true;
            }

            addTurn("Y", cube);
        }

        return false;
    }


    bool solveThirdLayerCorners(RubiksCube& cube)
    {

        // already solved case
        if (cube.getFace("front")[0][0] == cube.getFace("left")[0][2] && cube.getFace("front")[0][2] == cube.getFace("right")[0][0]
            && cube.getFace("right")[0][2] == cube.getFace("back")[0][0] && cube.getFace("back")[0][2] == cube.getFace("left")[0][0])
            return true;




        for (int i = 0; i < 4; i++)
        {
            if (cube.getFace("left")[0][0] == cube.getFace("left")[0][2])
            {
                if (cube.getFace("right")[0][0] == cube.getFace("right")[0][2])
                    return true;

                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                addTurn("U'", cube);
                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("U", cube);
                addTurn("L'", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                addTurn("U'", cube);
                addTurn("L", cube);
                return true;
            }

            addTurn("Y", cube);
            
        }

        addTurn("R", cube);
        addTurn("U", cube);
        addTurn("U", cube);
        addTurn("R'", cube);
        addTurn("U'", cube);
        addTurn("R", cube);
        addTurn("U", cube);
        addTurn("U", cube);
        addTurn("L'", cube);
        addTurn("U", cube);
        addTurn("R'", cube);
        addTurn("U'", cube);
        addTurn("L", cube);

        for (int i = 0; i < 4; i++)
        {
            if (cube.getFace("left")[0][0] == cube.getFace("left")[0][2])
            {
                if (cube.getFace("right")[0][0] == cube.getFace("right")[0][2])
                    return true;

                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                addTurn("U'", cube);
                addTurn("R", cube);
                addTurn("U", cube);
                addTurn("U", cube);
                addTurn("L'", cube);
                addTurn("U", cube);
                addTurn("R'", cube);
                addTurn("U'", cube);
                addTurn("L", cube);
                return true;
            }

            addTurn("Y", cube);

        }

        return false;
    }


    bool solveThirdLayerEdges(RubiksCube& cube)
    {
        // already solved case
        if (cube.getFace("left")[0][0] == cube.getFace("left")[0][1]
            && cube.getFace("front")[0][0] == cube.getFace("front")[0][1]
            && cube.getFace("right")[0][0] == cube.getFace("right")[0][1]
            && cube.getFace("back")[0][0] == cube.getFace("back")[0][1])
        {
            while (cube.getFace("front")[0][1] != cube.getFace("front")[1][1])
            {
                addTurn("U", cube);
            }

            return true;
        }




        int errorFlagCount = 0;
        int errorFlagCountLimit = 200;
        
        if (cube.getFace("left")[0][0] != cube.getFace("left")[0][1]
            && cube.getFace("front")[0][0] != cube.getFace("front")[0][1]
            && cube.getFace("right")[0][0] != cube.getFace("right")[0][1]
            && cube.getFace("back")[0][0] != cube.getFace("back")[0][1])
        {
            addTurn("R", cube);
            addTurn("R", cube);
            addTurn("U", cube);
            addTurn("R", cube);
            addTurn("U", cube);
            addTurn("R'", cube);
            addTurn("U'", cube);
            addTurn("R'", cube);
            addTurn("U'", cube);
            addTurn("R'", cube);
            addTurn("U", cube);
            addTurn("R'", cube);
        }

        while (cube.getFace("back")[0][1] != cube.getFace("back")[0][0])
        {
            addTurn("Y'", cube);
            errorFlagCount++;

            if (errorFlagCount > errorFlagCountLimit)
                return false;

        }

        if (cube.getFace("front")[0][1] == cube.getFace("left")[0][2]) // clock-wise permutation
        {
            addTurn("R", cube);
            addTurn("R", cube);
            addTurn("U", cube);
            addTurn("R", cube);
            addTurn("U", cube);
            addTurn("R'", cube);
            addTurn("U'", cube);
            addTurn("R'", cube);
            addTurn("U'", cube);
            addTurn("R'", cube);
            addTurn("U", cube);
            addTurn("R'", cube);
        }
        else // counter-clock-wise permutation
        {
            addTurn("R", cube);
            addTurn("U'", cube);
            addTurn("R", cube);
            addTurn("U", cube);
            addTurn("R", cube);
            addTurn("U", cube);
            addTurn("R", cube);
            addTurn("U'", cube);
            addTurn("R'", cube);
            addTurn("U'", cube);
            addTurn("R", cube);
            addTurn("R", cube);
        }

        errorFlagCount = 0;

        while (cube.getFace("front")[0][1] != cube.getFace("front")[1][1])
        {
            addTurn("U", cube);
            errorFlagCount++;

            if (errorFlagCount > errorFlagCountLimit)
                return false;
        }

        
        // final check
        bool isSolved = true;

        char faceColor;
        faceColor = cube.getFace("top")[0][0];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                isSolved = isSolved && (faceColor == cube.getFace("top")[i][j]);
        faceColor = cube.getFace("bottom")[0][0];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                isSolved = isSolved && (faceColor == cube.getFace("bottom")[i][j]);
        faceColor = cube.getFace("left")[0][0];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                isSolved = isSolved && (faceColor == cube.getFace("left")[i][j]);
        faceColor = cube.getFace("right")[0][0];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                isSolved = isSolved && (faceColor == cube.getFace("right")[i][j]);
        faceColor = cube.getFace("front")[0][0];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                isSolved = isSolved && (faceColor == cube.getFace("front")[i][j]);
        faceColor = cube.getFace("back")[0][0];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                isSolved = isSolved && (faceColor == cube.getFace("back")[i][j]);

        return isSolved;


    }



    void solve2(RubiksCube& cube)
    {

    }


    


public:
    RubiksCubeSolver()
    {
        
    }

    bool solve(RubiksCube& rubiksCube)
    {
        turnSequence.clear();
        cube.clone(rubiksCube);


        bool isSolvable = true;
        isSolvable = isSolvable && firstCheck(cube);

        if (!isSolvable)
            return false;


        solveCross(cube);
        solveFirstLayerCorners(cube);
        solveSecondLayer(cube);
        solveTopCross(cube);


        isSolvable = isSolvable && solveTopFace(cube) && solveThirdLayerCorners(cube) && solveThirdLayerEdges(cube);
        //isSolvable = isSolvable && solveTopFace(cube);
        //isSolvable = isSolvable;



        return isSolvable;
    }


    std::vector<std::string> solveResult()
    {
        return turnSequence;
    }

};


// rubiks cube solver class end



// objects
RubiksCube cube;




// camera positions and angles
float distance = 5.0f;

float xAngle = 10.0f;
float yAngle = 0.0f;
float zAngle = 0.0f;


float xPos = 1.5f - distance * sin(yAngle / 180 * M_PI);
float yPos = 1.5f - distance * cos(xAngle / 180 * M_PI);
float zPos = 1.5f - distance * sin(xAngle / 180 * M_PI);


// position and angle speed
float posSpeed = 1.0f;
float angleSpeed = 10.0f;


// turnQueryQueue
std::queue<std::string> turnQueryQueue;
bool queueRunning = false;
bool queueBusy = false;

float turnSpeed = 0.25f;  // turn speed in angles
float turnFinishedNumber = 90.f;
float turnProgressNumber = 0.0f;
std::string currentQuery;


RubiksCubeSolver solver;




void onKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
{

        // rotating
    if (key == GLFW_KEY_KP_8 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        xAngle += angleSpeed;
    }
    else if (key == GLFW_KEY_KP_2 && (action == GLFW_PRESS || action == GLFW_REPEAT ))
    {
        xAngle -= angleSpeed;
    }
    else if (key == GLFW_KEY_KP_9 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (distance > 1)
            distance -= posSpeed;
    }
    else if (key == GLFW_KEY_KP_3 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        distance += posSpeed;
    }
    else if (key == GLFW_KEY_KP_6 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        yAngle += angleSpeed;
    }
    else if (key == GLFW_KEY_KP_4 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        yAngle -= angleSpeed;
    }
    else if (key == GLFW_KEY_KP_5 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        yAngle -= angleSpeed;
    }
    else if (key == GLFW_KEY_I && (action == GLFW_PRESS))
    {
        std::cout << "Enter the configuration input file:\n";
        std::string fileName;
        std::cin >> fileName;
        cube.loadConfigurationFile(fileName);
    }
    else if (key == GLFW_KEY_O && (action == GLFW_PRESS))
    {
        std::cout << "Enter the configuration output file:\n";
        std::string fileName;
        std::cin >> fileName;
        cube.saveConfigurationFile(fileName);
    }
    else if (key == GLFW_KEY_K && (action == GLFW_PRESS))
    {

        int depth = 100;
        std::random_device randomDevice;
        std::mt19937 range(randomDevice());
        std::uniform_int_distribution<int> unbiased(0, 12);

        std::cout << "The scramble:\n";

        int randomNumber;
        for (int i = 0; i < depth; i++)
        {
            randomNumber = unbiased(range);

            if (randomNumber == 0)
            {
                turnQueryQueue.push("F");
                std::cout << "F ";
            }
            else if (randomNumber == 1)
            {
                turnQueryQueue.push("B");
                std::cout << "B ";
            }
            else if (randomNumber == 2)
            {
                turnQueryQueue.push("L");
                std::cout << "L ";
            }
            else if (randomNumber == 3)
            {
                turnQueryQueue.push("R");
                std::cout << "R ";
            }
            else if (randomNumber == 4)
            {
                turnQueryQueue.push("U");
                std::cout << "U ";
            }
            else if (randomNumber == 5)
            {
                turnQueryQueue.push("D");
                std::cout << "D ";
            }
            else if (randomNumber == 6)
            {
                turnQueryQueue.push("F'");
                std::cout << "F' ";
            }
            else if (randomNumber == 7)
            {
                turnQueryQueue.push("B'");
                std::cout << "B' ";
            }
            else if (randomNumber == 8)
            {
                turnQueryQueue.push("L'");
                std::cout << "L' ";
            }
            else if (randomNumber == 9)
            {
                turnQueryQueue.push("R'");
                std::cout << "R' ";
            }
            else if (randomNumber == 10)
            {
                turnQueryQueue.push("U'");
                std::cout << "U' ";
            }
            else if (randomNumber == 11)
            {
                turnQueryQueue.push("D'");
                std::cout << "D' ";
            }
        }
        std::cout << "\n";
        queueRunning = true;
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS))
    {
        if (solver.solve(cube))
        {
            
            auto resVect = solver.solveResult();

            std::cout << "The solution:\n";
            for (auto vect : resVect)
            {
                turnQueryQueue.push(vect);
                std::cout << vect << " ";
            }
            std::cout << "\n";

            queueRunning = true;

        }
        else
        {
            std::cout << "The cube is unsolvable\n";
        }

    }
    else if (key == GLFW_KEY_C && (action == GLFW_PRESS))
    {
        std::cout << "Current cube's state:\n";
        cube.outputCurrentState();
        std::cout << "\n\n";
    }
    else if (key == GLFW_KEY_N && (action == GLFW_PRESS))
    {
        queueRunning = false;
        queueBusy = false;
        while (!turnQueryQueue.empty())
            turnQueryQueue.pop();

        RubiksCube cube2;
        cube.clone(cube2);
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (mods == GLFW_MOD_SHIFT)
            turnQueryQueue.push("F'");
        else
            turnQueryQueue.push("F");
        queueRunning = true;
    }
    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        if (mods == GLFW_MOD_SHIFT)
            turnQueryQueue.push("B'");
        else
            turnQueryQueue.push("B");
        queueRunning = true;
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (mods == GLFW_MOD_SHIFT)
            turnQueryQueue.push("L'");
        else
            turnQueryQueue.push("L");
        queueRunning = true;
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        if (mods == GLFW_MOD_SHIFT)
            turnQueryQueue.push("R'");
        else
            turnQueryQueue.push("R");
        queueRunning = true;
    }
    else if (key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        if (mods == GLFW_MOD_SHIFT)
            turnQueryQueue.push("U'");
        else
            turnQueryQueue.push("U");
        queueRunning = true;
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        if (mods == GLFW_MOD_SHIFT)
            turnQueryQueue.push("D'");
        else                     
            turnQueryQueue.push("D");
        queueRunning = true;
    }
    else if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        if (mods == GLFW_MOD_SHIFT)
            turnQueryQueue.push("X'");
        else                     
            turnQueryQueue.push("X");
        queueRunning = true;
    }
    else if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        if (mods == GLFW_MOD_SHIFT)
            turnQueryQueue.push("Y'");
        else                     
            turnQueryQueue.push("Y");
        queueRunning = true;
    }
    else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        if (mods == GLFW_MOD_SHIFT)
            turnQueryQueue.push("Z'");
        else
            turnQueryQueue.push("Z");
        queueRunning = true;
    }
}


void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}





int main(void)
{

      
    // ----- INITIALIZING ----- 

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // ----- WINDOW CREATING -----
    window = glfwCreateWindow(800, 600, "App3", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, onKeyDown);

    glfwMakeContextCurrent(window);
    glFrustum(-1, 1, -1, 1, 2, 60);
    glEnable(GL_DEPTH_TEST);



    // ----- MAIN LOOP -----
    while (!glfwWindowShouldClose(window))
    {
        // --- RENDERING -----

        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();


        float delta = -1.5f;
        yPos = delta + distance * sin(-xAngle / 180 * M_PI);
        xPos = delta + distance * sin(yAngle / 180 * M_PI) * cos(xAngle / 180 * M_PI);
        zPos = delta - distance * cos(yAngle / 180 * M_PI) * cos(xAngle / 180 * M_PI);

        glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
        glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
        glRotatef(zAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef(xPos, yPos, zPos);

        cube.draw();

        // queue handling
        if (queueRunning)
        {

            if (!queueBusy && turnQueryQueue.empty())
            {
                queueRunning = false;
            }


            if (queueBusy)
            {
                cube.turn(currentQuery, turnSpeed);
                turnProgressNumber += turnSpeed;

                if (turnProgressNumber == turnFinishedNumber) {
                    queueBusy = false;
                    cube.updateState(currentQuery);


                }

            }
            else if (!queueBusy && queueRunning)
            {

                queueBusy = true;

                turnProgressNumber = 0;
                currentQuery = turnQueryQueue.front();
                turnQueryQueue.pop();

            }

        }

        
        glPopMatrix();


        // ----- AFTER RENDERING -----


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}