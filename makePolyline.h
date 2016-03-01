
//
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
//
//

#ifndef __MAKEPOLYLINE_H__
#define __MAKEPOLYLINE_H__

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "mesh.h"
#include "polyline.h"
#include "zipper.h"
#include "subdivison.h"

using namespace glm;
using namespace std;

//////////////////////////////////////////////////////////////////////
// MakePolyline Class -- Create Initial Meshes.
Mesh initPolyline();

Mesh initPolyline1();

Mesh initPolyline2();

Mesh initPolyline4();

Mesh initPolyline8();

Mesh initPolyline9();

Mesh initPolyline3();

Mesh initPolyline5();

Mesh initPolyline6();

#endif // __MAKEPOLYLINE_H__
