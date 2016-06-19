#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include "triangulate.h"

struct Edge {
    Edge(const glm::vec3 &p1, const glm::vec3 &p2) : p1(p1), p2(p2) {};
    Edge(const Edge &e) : p1(e.p1), p2(e.p2) {};
    glm::vec3 p1, p2;
};

inline bool operator == (const Edge & e1, const Edge & e2)
{
	return 	(e1.p1 == e2.p1 && e1.p2 == e2.p2) ||
			(e1.p1 == e2.p2 && e1.p2 == e2.p1);
}

struct Triangle {
    glm::vec3 p1, p2, p3;
    Edge e1 = {p1, p2};
    Edge e2 = {p2, p3};
    Edge e3 = {p3, p1};

    bool containsVertex(const glm::vec3 &v);
    bool circumCircleContains(const glm::vec3 &v);
};

inline bool operator == (const Triangle &t1, const Triangle &t2)
{
	return	(t1.p1 == t2.p1 || t1.p1 == t2.p2 || t1.p1 == t2.p3) &&
			(t1.p2 == t2.p1 || t1.p2 == t2.p2 || t1.p2 == t2.p3) && 
			(t1.p3 == t2.p1 || t1.p3 == t2.p2 || t1.p3 == t2.p3);
}

bool Triangle::containsVertex(const glm::vec3 &v)
{
	return p1 == v || p2 == v || p3 == v; 
}

bool Triangle::circumCircleContains(const glm::vec3 &v)
{
	float ab = (p1.x * p1.x) + (p1.z * p1.z);
	float cd = (p2.x * p2.x) + (p2.z * p2.z);
	float ef = (p3.x * p3.x) + (p3.z * p3.z);

	float circum_x = (ab * (p3.z - p2.z) + cd * (p1.z - p3.z) + ef * (p2.z - p1.z))
        / (p1.x * (p3.z - p2.z) + p2.x * (p1.z - p3.z) + p3.x * (p2.z - p1.z)) / 2.f;
	float curcum_z = (ab * (p3.x - p2.x) + cd * (p1.x - p3.x) + ef * (p2.x - p1.x))
        / (p1.z * (p3.x - p2.x) + p2.z * (p1.x - p3.x) + p3.z * (p2.x - p1.x)) / 2.f;
	float circum_radius_sq = ((p1.x - circum_x) * (p1.x - circum_x))
            + ((p1.z - curcum_z) * (p1.z - curcum_z));

	float dist_sq = ((v.x - circum_x) * (v.x - circum_x)) + ((v.z - curcum_z) * (v.z - curcum_z));
	return dist_sq <= circum_radius_sq;
}

void triangulate(glm::vec3 *vertices, int &nv)
{
    std::vector<Triangle> triangles;
    std::vector<Edge> edges;

	// Determinate the super triangle
	float minX = vertices[0].x;
	float minY = vertices[0].y;
	float minZ = vertices[0].z;
	float maxX = minX;
	float maxY = minY;
	float maxZ = minZ;

	for(int i = 0; i < nv; ++i) 
	{
		if (vertices[i].x < minX) minX = vertices[i].x;
		if (vertices[i].y < minY) minY = vertices[i].y;
    	if (vertices[i].z < minZ) minZ = vertices[i].z;
    	if (vertices[i].x > maxX) maxX = vertices[i].x;
    	if (vertices[i].y > maxY) maxY = vertices[i].y;
    	if (vertices[i].z > maxZ) maxZ = vertices[i].z;
	}
	
	float dx = maxX - minX;
	float dz = maxZ - minZ;
	float deltaMax = std::max(dx, dz);
	float midx = (minX + maxX) / 2.f;
	float midy = (minY + maxY) / 2.f;
	float midz = (minZ + maxZ) / 2.f;

	glm::vec3 p1(midx - 20 * deltaMax, midy, midz - deltaMax);
	glm::vec3 p2(midx, midy, midz + 20 * deltaMax);
	glm::vec3 p3(midx + 20 * deltaMax, midy, midz - deltaMax);	
	
	// Create a list of triangles, and add the supertriangle in it
	triangles.push_back(Triangle{p1, p2, p3});

    for (int i = 0; i < nv; i++) {
        glm::vec3 point = vertices[i];

		std::vector<Triangle> badTriangles;
		std::vector<Edge> polygon;

		for(auto triangle : triangles) {
			if(triangle.circumCircleContains(point)) {
				badTriangles.push_back(triangle);
				polygon.push_back(triangle.e1);	
				polygon.push_back(triangle.e2);	
				polygon.push_back(triangle.e3);	
			}
		}

		triangles.erase(std::remove_if(begin(triangles), end(triangles), [badTriangles](Triangle &t){
			for(auto badTriangle : badTriangles) {	
				if(badTriangle == t)
					return true;		
			}
			return false;
		}), end(triangles));

		std::vector<Edge> badEdges;
		for(auto e1 = begin(polygon); e1 != end(polygon); e1++) {
			for(auto e2 = begin(polygon); e2 != end(polygon); e2++) {
				if(e1 == e2)
					continue;
				
				if(*e1 == *e2)
				{
					badEdges.push_back(*e1);	
					badEdges.push_back(*e2);	
				}
			}
		}

		polygon.erase(std::remove_if(begin(polygon), end(polygon), [badEdges](Edge &e){
			for(auto badEdge : badEdges) {
				if(badEdge == e)
					return true;
			}
			return false;
		}), end(polygon));

		for(auto edge : polygon)
			triangles.push_back(Triangle{edge.p1, edge.p2, point});
	
	}

	triangles.erase(std::remove_if(begin(triangles), end(triangles), [p1, p2, p3](Triangle &t){
		return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
	}), end(triangles));

    nv = 3 * triangles.size();
    vertices = (glm::vec3*)realloc(vertices, nv * sizeof(glm::vec3));

    int i = 0;
	for(auto triangle : triangles) {
        vertices[i++] = triangle.p1;
        vertices[i++] = triangle.p2;
        vertices[i++] = triangle.p3;
    }
}

