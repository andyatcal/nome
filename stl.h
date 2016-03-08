//
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
//
//

#ifndef __STL_H__
#define __STL_H__

#include <vector>
#include <iomanip>
#include <fstream>
#include "mesh.h"

using namespace glm;
using namespace std;

// Forward declarations
class Vertex;
class Face;

//////////////////////////////////////////////////////////////////////
// STL Class -- Input and Output with STL Format Files.
//
class STL{
public:
    STL(){};
    void STLOutput(vector<Mesh> &meshes, string outputSTL);
    void STLOutput(Mesh &mesh, string outputSTL);
};

// Get a triangular surface normal.
// @param v1 v2 v3: three vertices of the triangle.
vec3 getTriFaceNormal(Vertex * va, Vertex * vb, Vertex * vc);

#endif// __STL_H__
