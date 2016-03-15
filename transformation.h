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
#include "polyline.h"

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Transformation Class -- A class to create a new mesh by transformation.
/**
 * @brief transform: Create a new Mesh given the
 * transformation matrix.
 * @param mesh: The mesh for transformation.
 * @param matrix: The matrix for transformation.
 */
void transform(Mesh &mesh, mat4 matrix);

/**
 * @brief transform: Create a new PolyLine given the
 * transformation matrix.
 * @param line: The line for transformation.
 * @param matrix: The matrix for transformation.
 */
void transform(PolyLine &line, mat4 matrix);

/**
* @brief krotate: Rotate a current mesh.
* @param rot_axis: The rotation axis as a 3D vector
* @param radian: The rotation angle, in radius
* @return transformation matrix for this rotation.
*/
mat4 krotate(vec3 rot_axis, float radian);

/**
* @brief kscale: Scale a current mesh.
* @param scales: The scaling factor as a 3D vector.
* @return transformation matrix for this scaling.
*/
mat4 kscale(vec3 scales);

/**
* @brief ktranslate: Translate a current mesh.
* @param translation: The translation as a 3D vector.
* @return transformation matrix for this translation.
*/
mat4 ktranslate(vec3 translation);
/**
* @brief mirror: Mirror a current mesh.
* @param mirror_plane: The mirror plane as a 4D vector.
* @return transformation matrix for this mirror operation.
*/
mat4 kmirror(vec4 mirror_plane);
/**
 * @brief meshCopy: Make a copy of current mesh.
 * @param mesh: The mesh to be copied.
 * @return The copied mesh.
 */
Mesh meshCopy(Mesh & mesh);

/**
 * @brief polylineCopy: Make a copy of current line.
 * @param line: The line to be copied.
 * @return The copied line.
 */
PolyLine polylineCopy(PolyLine &line);

#endif // TRANSFORMATION_H
