#include <iostream>
#include <vector>
#include <math.h>
#include <iterator>

#define M_PI 3.14159265358979323846



class Point {
	double x, y;
public:
	Point() {}
	Point(double x, double y);
	Point(const Point& point2);

	double getX() const {
		return x;
	}
	double getY() const {
		return y;
	}
	void setX(double x) {
		this->x = x;
	}
	void setY(double x) {
		this->y = y;
	}
	
	Point& operator=(const Point& point2);
};

Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
} 

Point::Point(const Point& point2) {
	x = point2.getX();
	y = point2.getY();
}

Point& Point::operator=(const Point& point2) {
	if (this != &point2) {
		x = point2.getX();
		y = point2.getY();
	}
}





class Polyline {
protected:
	std::vector<Point> points;
	Polyline() {}
public:
	Polyline(Point pointsArray[], int pointsNumber);

	virtual double getLength() const;
	
	void addPoint(Point point);
	void setPoint(int index, Point point);
	void removePoint(int index);
	Point getPoint(int index);
};


Polyline::Polyline(Point pointsArray[], int pointsNumber) {
	if (pointsNumber < 1)
		return;
	for (int i = 0; i < pointsNumber; i++) {
		points.push_back(pointsArray[i]);
	}
}

double Polyline::getLength() const {
	double res = 0;
	
	if (points.size() == 1)
		return 0;
	
	for(std::vector<Point>::const_iterator it = points.begin() + 1; it != points.end(); it++) {
		Point prevPoint = *(it - 1);
		Point currentPoint = *it;
		
    	res += sqrt( pow(currentPoint.getX() - prevPoint.getX(), 2) + pow(currentPoint.getY() - prevPoint.getY(), 2));
	}
	
	return res;
}

void Polyline::addPoint(Point point) {
	points.push_back(point);
}

void Polyline::setPoint(int index, Point point) {
	points[index] = point;
}

void Polyline::removePoint(int index) {
	points.erase(points.begin() + index);
}

Point Polyline::getPoint(int index) {
	return points[index];
}





class ClosedPolyline : public Polyline {
	using Polyline::Polyline;
protected:
	ClosedPolyline() {}
public:
	double getLength() const;
};

double ClosedPolyline::getLength() const {
	if (points.size() <= 2)
		return Polyline::getLength();
	
	return Polyline::getLength() + 
		sqrt( pow(points[points.size() - 1].getX() - points[0].getX(), 2) + pow(points[points.size() - 1].getY() - points[0].getY(), 2));
}


class Polygon : public ClosedPolyline {
	using ClosedPolyline::ClosedPolyline;
protected:
	Polygon() {}
public:
	virtual double getPerimeter() const;
	virtual double getArea() const;
};


double Polygon::getPerimeter() const {
	return ClosedPolyline::getLength();
}

double Polygon::getArea() const {
	double res = 0;
	
	if (points.size() <= 2)
		return 0;
	
	for (int i = 0; i < points.size() - 1; i++) {
		res += points[i].getX() * points[i + 1].getY() - points[i].getY() * points[i + 1].getX();
	}
	int lastIndex = points.size() - 1;
	res += points[lastIndex].getX() * points[0].getY() - points[lastIndex].getY() * points[0].getX();
	return 0.5 * abs(res);
	
}


class Triangle : public Polygon {
	
public:
	Triangle(Point pointsArray[], int pointsNumber);
	Triangle(Point point1, Point point2, Point point3);	
};

Triangle::Triangle(Point pointsArray[], int pointsNumber) {
	if (pointsNumber != 3)
		return;
	
	points.push_back(pointsArray[0]);
	points.push_back(pointsArray[1]);
	points.push_back(pointsArray[2]);
}

Triangle::Triangle(Point point1, Point point2, Point point3) {
	points.push_back(point1);
	points.push_back(point2);
	points.push_back(point3);	
}





class Trapezoid : public Polygon {
	Point b1p1;
	Point b1p2;
	Point b2p1;
	Point b2p2;

	bool checkParallel(Point base1Point1, Point base1Point2, Point base2Point1, Point base2Point2);

public:
	void set(Point base1Point1, Point base1Point2, Point base2Point1, Point base2Point2);
	
	Trapezoid(Point base1Point1, Point base1Point2, Point base2Point1, Point base2Point2);
	double getPerimeter() const;
	double getArea() const;
	
	Point getB1P1() const {
		return b1p1;
	}

	Point getB1P2() const {
		return b1p2;
	}
	
	Point getB2P1() const {
		return b2p1;
	}
	
	Point getB2P2() const {
		return b2p2;
	}
};

bool Trapezoid::checkParallel(Point base1Point1, Point base1Point2, Point base2Point1, Point base2Point2) {
	bool parallel = false;
	if (base1Point2.getX() - base1Point1.getX() == 0 && base1Point2.getX() - base1Point1.getX() == 0)
		parallel = true;
	
	parallel =
		(base1Point2.getY() - base1Point1.getY()) / (base1Point2.getX() - base1Point1.getX())
	 == (base2Point2.getY() - base2Point1.getY()) / (base2Point2.getX() - base2Point1.getX());
	 
	return parallel;
}

void Trapezoid::set(Point base1Point1, Point base1Point2, Point base2Point1, Point base2Point2) {
	if (checkParallel(base1Point1, base1Point2, base2Point1, base2Point2)) {
		b1p1 = base1Point1;
		b1p2 = base1Point2;
		b2p1 = base2Point1;
		b2p2 = base2Point2;
	}
}



Trapezoid::Trapezoid(Point base1Point1, Point base1Point2, Point base2Point1, Point base2Point2) {
	set(base1Point1, base1Point2, base2Point1, base2Point2);	
}


double Trapezoid::getPerimeter() const {
	return sqrt( pow(b1p2.getX() - b1p1.getX(), 2) + pow(b1p2.getY() - b1p1.getY(), 2))
	+ sqrt( pow(b1p1.getX() - b2p1.getX(), 2) + pow(b1p1.getY() - b2p1.getY(), 2))
	+ sqrt( pow(b1p1.getX() - b1p2.getX(), 2) + pow(b1p1.getY() - b1p2.getY(), 2))
	+ sqrt( pow(b1p2.getX() - b2p2.getX(), 2) + pow(b1p2.getY() - b2p2.getY(), 2));
}

double Trapezoid::getArea() const {
	double res = b1p1.getX() * b1p2.getY() - b1p1.getY() * b1p2.getX()
			   + b1p2.getX() * b2p2.getY() - b1p2.getY() * b2p2.getX()
			   + b2p2.getX() * b2p1.getY() - b2p2.getY() * b2p1.getX()
			   + b2p1.getX() * b1p1.getY() - b2p1.getY() * b1p1.getX();
			   
	return 0.5 * abs(res);
	
}


class RegularPolygon : public Polygon {
	Point center;
	double radius;
	int edges;
	
public:
	RegularPolygon(Point center, double radius, int edges);
	
	Point getCenter() const {
		return center;
	}
	double getRadius() const {
		return radius;
	}
	int getEdges() const {
		return edges;
	}
	
	void setCenter(Point point) {
		center = point;
	}
	void setRadius(double value) {
		if (radius > 0)
			radius = value;
	}
	void setEdges(int number) {
		if (number >= 3)
			edges = number;
	}
	
	double getPerimeter() const;
	double getArea() const;
};


double RegularPolygon::getPerimeter() const {
	double edgeLength = radius * 2 * sin(M_PI / edges);
	return edgeLength * edges;
}

double RegularPolygon::getArea() const {
	return 0.5 * edges * radius * radius * sin(2 * M_PI / edges);
}



RegularPolygon::RegularPolygon(Point center, double radius, int edges) {
	if (edges < 3 || radius <= 0)
		return;
	
	this->center = center;
	this->radius = radius;
	this->edges = edges;
}





int main(int argc, char **argv) {
	
	
	// point
	Point point1(4, 3);
	std::cout << "point1 = " << point1.getX() << " " << point1.getY() << std::endl;
	
	Point point2(point1);
	point1.setX(12);
	std::cout << "point1 = " << point1.getX() << " " << point1.getY() << std::endl;
	std::cout << "point2 = " << point2.getX() << " " << point2.getY() << std::endl;	
	
	
	
	// polyline
	Point pointsArray1[] = {Point(0, 0), Point(-3, 0), Point(0, 4)};
	
	Polyline line1(pointsArray1, 3);
	std::cout << "length of line1 = " << line1.getLength() << std::endl;
	line1.removePoint(1);
	std::cout << "new length of line1 = " << line1.getLength() << std::endl;	
	
	
	
	// closed polyline
	ClosedPolyline line2(pointsArray1, 3);
	std::cout << "length of line2 = " << line2.getLength() << std::endl;
	
	
	
	// polygon
	Polygon polygon1(pointsArray1, 3);
	std::cout << "perimeter of polygon1 = " << polygon1.getPerimeter() << std::endl;
	std::cout << "area of polygon1 = " << polygon1.getArea() << std::endl;	
	
	
	
	// trapezoid
	Trapezoid trapezoid1(Point(-3, 0), Point(5, 0), Point(-2, 4), Point(1, 4));
	std::cout << "perimeter of trapezoid1 = " << trapezoid1.getPerimeter() << std::endl;
	std::cout << "area of trapezoid1 = " << trapezoid1.getArea() << std::endl;
	
	
	
	// regular polygon
	RegularPolygon regPolygon(Point(0, 0), 5, 724);
	std::cout << "perimeter of regPolygon = " << regPolygon.getPerimeter() << std::endl;
	std::cout << "area of regPolygon = " << regPolygon.getArea() << std::endl;
	
	
	// polymorphic array
	Polygon *polygons[3] = {&polygon1, &trapezoid1, &regPolygon};
	polygons[1]->getPerimeter();
	
	for (int i = 0; i < 3; i++) {
		std::cout << "perimeter of polygon #" << i << " is " << polygons[i]->getPerimeter() << std::endl;
		std::cout << "area of polygon #" << i << " is " << polygons[i]->getArea() << std::endl;
	}
	
	
	return 0;
}
