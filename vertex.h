/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class Edge;
class Mesh;

/**
 * @brief The Vertex class. Build for vertex in winged-
 * edge data structure.
 */
class Vertex{
public:
    Vertex();
    /**
     * @brief Vertex: Constructor of class Vertex.
     * @param x: x coordinate.
     * @param y: y coordinate.
     * @param z: z coordinate.
     * @param ID: The vertex ID.
     */
    Vertex(float x, float y, float z, unsigned long ID);
    /* The position. */
    vec3 position;
    /* Vertex normal. */
    vec3 normal;
    /* One out-going halfedge of this vertex.*/
    Edge * oneEdge;
    /* A tracking identifier. Generally the index from global vertex list.*/
    unsigned long ID;
    /* True if it is the end of any Mobius edge.*/
    bool onMobius;
    /* A pointer to the vertex point in subdivision.*/
    Vertex * vertexPoint;
    /* A pointer to the positive offset vertex in offsetting. */
    Vertex * posOffset;
    /* A pointer to the negative offset vertex in offsetting. */
    Vertex * negOffset;
    /* Indicator of whether this vertex is selected.*/
    bool selected;
};

#endif // __VERTEX_H__
