/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include <vector>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include "mesh.h"

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Transformation Class -- A class to create a new mesh by transformation.

// Create a new Mesh given the transformation matrix
// @param mesh, the mesh for transformation
// @param matrix, the matrix for transformation
void transform(Mesh & mesh, mat4 matrix);

// Rotate a current mesh.
// @param rot_axis, the rotation axis as a 3D vector
// @param radian, the rotation angle
// @return transformation matrix for this rotation
mat4 krotate(vec3 rot_axis, float radian);

// Scale a current mesh.
// @param scales, the scaling factor as a 3D vector
// @return transformation matrix for this scaling
mat4 kscale(vec3 scales);

// Translate a current mesh.
// @param translation, the translation as a 3D vector
// @return transformation matrix for this translation
mat4 ktranslate(vec3 translation);

mat4 kmirror(vec4 mirror_plane);

Mesh meshCopy(Mesh & mesh);

#endif // TRANSFORMATION_H
