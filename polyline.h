/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef __POLYLINE_H__
#define __POLYLINE_H__

#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Polyline Class -- A Polyline is a list of vertices
class PolyLine {
public:
    vector<Vertex*> vertices;
    bool isLoop;
    // Constructor
    PolyLine();
    // Draw this polyline in OpenGL;
    void drawLine();
    // Return the skewness of points in this polyline.
    vector<float> skewness();
};

#endif // __POLYLINE_H__
