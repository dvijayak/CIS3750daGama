#include "Polygon.hpp"

std::ostream& operator<< (std::ostream& os, const Polygon3F& polygon)
{
	os << "( ";
	for (int i = 0; i < polygon.nVert; ++i)
	{
		os << polygon.vertices[i];
		if (i != polygon.nVert - 1)
		{
			os << ", ";
		}
	}
	os << " )";
	return os;
}

std::ostream& operator<< (std::ostream& os, const std::vector<Polygon3F*>& v)
{
	int size = v.size();
	if (size > 0)
	{
		os << "[ ";
		for (int i = 0; i < size; ++i)
		{
			os << *v[i];
			if (i != size - 1)
			{
				os << ", " << std::endl;
			}
		}
		os << " ]";
	}
	return os;
}


// static members
std::vector<Polygon3F*> Polygon3FFactory::s_polygonSoup = std::vector<Polygon3F*>();

Polygon3FFactory::Polygon3FFactory (){}
Polygon3FFactory::~Polygon3FFactory ()
{
	// Destroy all polygons ever created by this factory
	for (std::vector<Polygon3F*>::iterator it; it != s_polygonSoup.end(); ++it)
	{
		delete *it;
	}
}

void Polygon3FFactory::AssociateEdges (Polygon3F* polygon)
{
	polygon->edges = std::vector<LineSegment3F>(polygon->nVert);
	for (int i = 0; i < polygon->nVert; ++i)
	{
		polygon->edges[i] = LineSegment3F(
			polygon->vertices[i],
			polygon->vertices[ (i + 1) % polygon->nVert ] // Last edge's end-point is the first vertex
			);
	}
}

Polygon3F* Polygon3FFactory::CreateQuad (Point3F v1, Point3F v2, Point3F v3, Point3F v4)
{
	Polygon3F* polygon = new Polygon3F;
	polygon->nVert = 4;

	// Set vertices
	polygon->vertices = std::vector<Point3F>(4);
	polygon->vertices[0] = v1;
	polygon->vertices[1] = v2;
	polygon->vertices[2] = v3;
	polygon->vertices[3] = v4;

	// Associate edges
	AssociateEdges(polygon);
		
	// Store reference
	s_polygonSoup.push_back(polygon);

	return polygon;
}

Polygon3F* Polygon3FFactory::CreatePolygon (Point3F vertices[], int nVert)
{
	Polygon3F* polygon = 0;

	if (nVert >= 3)
	{
		// Create a polygon with at least 3 vertices
		polygon = new Polygon3F;
		polygon->nVert = nVert;

		// Set vertices
		polygon->vertices = std::vector<Point3F>(nVert);
		for (int i = 0; i < nVert; ++i)
		{
			polygon->vertices[i] = vertices[i];
		}

		// Associate edges
		AssociateEdges(polygon);

		// Store reference
		s_polygonSoup.push_back(polygon);
	}

	return polygon;
}

Polygon3F* Polygon3FFactory::CreatePolygon (std::vector<Point3F> vertices)
{
	Polygon3F* polygon = 0;

	if (!vertices.empty() && vertices.size() >= 3)
	{
		// Create a polygon with at least 3 vertices
		polygon = new Polygon3F;
		polygon->nVert = vertices.size();

		// Set vertices
		polygon->vertices = vertices;

		// Associate edges
		AssociateEdges(polygon);

		// Store reference
		s_polygonSoup.push_back(polygon);
	}

	return polygon;
}

std::vector<Polygon3F*>& Polygon3FFactory::GetPolygonSoup ()
{
	return s_polygonSoup;
}
