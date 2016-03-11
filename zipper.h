/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

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
    Zipper();
    /**
     * @brief zip: Greedy zippering two polylines. It starts from both sides
     * of the polyline and will greedily minimize the distance between vertices
     * from the two polylines.
     * @param b1: the first border polyline.
     * @param b2: the second border polyline.
     * @param trianglePenalty: the weight to penalize forming triangles.
     * @return Mesh build by zipping this two polylines.
     */
    Mesh zip(PolyLine * b1, PolyLine * b2, float trianglePenalty = 1.3);
    /**
     * @brief zip_skewness: zippering two polylines. It starts from both sides
     * of the polyline and will greedily by balancing the skewness between vertices
     * from the two polylines.
     * @param b1: the first border polyline.
     * @param b2: the second border polyline.
     * @param trianglePenalty: the weight to penalize forming triangles.
     * @return Mesh build by zipping this two polylines.
     */
    Mesh zip_skewness(PolyLine * b1, PolyLine * b2, float penalty = 1.3);
};

#endif //__ZIPPER_H__
