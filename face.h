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

using namespace glm;
using namespace std;

// Forward declarations
class Vertex;

//////////////////////////////////////////////////////////////////////
// Face Class -- Polygon Face for Catmull-Clark Subdivision.
//
class Face{
public:
    // The surface normal of this face.
    vec3 normal;
    // A pointer to one edge that constructs this face.
    Edge * oneEdge;
    // A pointer to the face point after makeFacePoints in subdivision.
    Vertex * facePoint;
    // Constructor.
    Face();
    // Test if this face is selected
    bool selected;
    // FaceID, should be an unique int
    int id;

};

#endif // __FACE_H__
