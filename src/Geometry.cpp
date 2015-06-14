#include "Geometry.hpp"

std::ostream& operator<< (std::ostream& os, const Point3F& point)
{
	os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
	return os;
}

std::ostream& operator<< (std::ostream& os, const LineSegment3F& line)
{
	os << "(" << line.start << ", " << line.end << ")";
	return os;
}
