#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "global.hpp"

struct Point3F;
typedef Point3F Point;
struct LineSegment3F;
typedef LineSegment3F LineSegment;

//// Points

struct Point3F
{
	Point3F (const float _x=0.0, const float _y=0.0, const float _z=0.0) : x(_x), y(_y), z(_z) {}
	Point3F (const Point3F& other) : x(other.x), y(other.y), z(other.z) {}
	Point3F& operator= (const Point3F& other) { x = other.x; y = other.y; z = other.z; return *this; }
	~Point3F () {}

	float x, y, z;
};

std::ostream& operator<< (std::ostream& os, const Point3F& point);

//// Lines

struct LineSegment3F
{
	LineSegment3F (const Point3F& _start=Point3F(), const Point3F& _end=Point3F() ) : start(_start), end(_end) {}
	LineSegment3F (const LineSegment3F& other) : start(other.start), end(other.end) {}
	LineSegment3F& operator= (const LineSegment3F& other) { start = other.start; end = other.end; return *this; }
	~LineSegment3F () {}

	Point3F start;
	Point3F end;
};

std::ostream& operator<< (std::ostream& os, const LineSegment3F& line);

//// Polygons

#include "Polygon.hpp"

/// Geometry

class Geometry {};
// {
// public:
// 	Geometry (){};
// 	~Geometry (){};
// };

#endif
