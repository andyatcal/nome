//
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
//
//

#ifndef __ZIPPER_H__
#define __ZIPPER_H__

#include <vector>
#include <math.h>
#include "glm/glm.hpp"
#include "mesh.h"
#include "polyline.h"

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Polyline Class -- A Polyline is a list of vertices
class Zipper
{
public:
    // Constructor
    Zipper();
    // Function zipper, zippering two polylines.
    Mesh zip(Polyline * b1, Polyline * b2, float trianglePenalty = 1.3);
    Mesh zip_skewness(Polyline * b1, Polyline * b2, float penalty = 1.3);
};

#endif //__ZIPPER_H__
