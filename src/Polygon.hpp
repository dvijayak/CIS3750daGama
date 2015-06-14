#ifndef POLYGON_H
#define POLYGON_H

#include "global.hpp"

#include "Geometry.hpp"

struct Polygon3F
{
	int nVert; // Number of vertices
	std::vector<Point3F> vertices; // Vertices
	std::vector<LineSegment3F> edges; // Edges
};

std::ostream& operator<< (std::ostream& os, const Polygon3F& polygon);
std::ostream& operator<< (std::ostream& os, const std::vector<Polygon3F*>& v);

class Polygon3FFactory
{
public:
	Polygon3FFactory ();
	~Polygon3FFactory ();

	static Polygon3F* CreateQuad (Point3F v1, Point3F v2, Point3F v3, Point3F v4);

	static Polygon3F* CreatePolygon (Point3F vertices[], int nVert = 4);
	static Polygon3F* CreatePolygon (std::vector<Point3F> vertices);

	static std::vector<Polygon3F*>& GetPolygonSoup ();

private:
	static void AssociateEdges (Polygon3F* polygon);

	static std::vector<Polygon3F*> s_polygonSoup; // Maintains references to all polygons ever created by the factory; also used for RAII

public:	
	// Convenience
	static Polygon3F* CreateQuad (Point3F v[4]);
};

inline Polygon3F* Polygon3FFactory::CreateQuad (Point3F v[4])
{
	return CreateQuad(v[0], v[1], v[2], v[3]);
}

#endif
