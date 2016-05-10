/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef __FACE_H__
#define __FACE_H__

#include <glm/glm.hpp>
#include <vector>
#include "edge.h"
#include "vertex.h"
#include "mesh.h"
using namespace glm;
using namespace std;

/**
 * @brief The Face class. The face class build for winged-
 * edge data structure. A face is constructed by a seqence of
 * vertices/edges.
 */
class Face
{
public:
    Face();
    /* The face normal.*/
    vec3 normal;
    /* Pointer to one edge in this face.*/
    Edge *oneEdge;
    /* Pointer to the face point in subdivision. */
    Vertex *facePoint;
    /* Indicator of whether this face is selected.*/
    bool selected;
    /* FaceID, is the index from the global face list.*/
    int id;
};

#endif // __FACE_H__
