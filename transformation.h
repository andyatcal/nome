/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include <vector>
#include <string>
#include <unordered_map>
#include "utils.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Transformation Class -- A class to create a new mesh by transformation.
class Transformation
{
public:
    Transformation();
    Transformation(int type, float x, float y, float z);
    Transformation(int type, float x, float y, float z, float w);
    Transformation(int type, unordered_map<string, Parameter> *params, string input1, string input2 = "");
    /**
     * @brief type: The type of this transformation.
     * 0: Nothing, matrix is an identity matrix.
     * 1: Rotation
     * 2: Scaling
     * 3: Translation
     * 4: Mirroring
     */
    int type;
    /**
     * @brief x: The first parameter of this transformation.
     * Roation: the x for rotation axis
     * Scale: scaling factor in x direction.
     * Translation: translation in x direction.
     * Mirroring: the constant for x of mirroring plane.
     */
    float x;
    /**
     * @brief y: The second parameter of this transformation.
     * Roation: the y for rotation axis
     * Scale: scaling factor in y direction.
     * Translation: translation in y direction.
     * Mirroring: the constant for y of mirroring plane.
     */
    float y;
    /**
     * @brief z: The third parameter of this transformation.
     * Roation: the z for rotation axis
     * Scale: scaling factor in z direction.
     * Translation: translation in z direction.
     * Mirroring: the constant for z of mirroring plane.
     */
    float z;
    /**
     * @brief x: The first parameter of this transformation.
     * Roation: angle for rotation axis
     * Scale: don't have this parameter.
     * Translation: don't have this parameter
     * Mirroring: the constant of intercept of mirroring plane.
     */
    float w;
    /* Indicate if this transformation is parametric. */
    bool isParametric;
    /* The string expression of parameter x. */
    string x_expr;
    /* The string expression of parameter y. */
    string y_expr;
    /* The string expression of parameter z. */
    string z_expr;
    /* The string expression of parameter w. */
    string w_expr;
    /* A pointer to the global parameter. */
    unordered_map<string, Parameter> *params;
    /* Set the global parameter pointer for this mesh. */
    void setGlobalParameter(unordered_map<string, Parameter> *params);
    /* The stored transformation matrix.*/
    mat4 matrix;
    /**
    * @brief rotate: Create the rotation matrix based on parameters.
    */
    void rotate();

    /**
    * @brief scale: Create the scaling matrix based on parameters.
    */
    void scale();

    /**
    * @brief translate: Create the translation matrix based on parameters.
    */
    void translate();
    /**
    * @brief mirror: Create the mirror matrix based on parameters.
    */
    void mirror();
    /**
     * @brief getMatrix: Get a 4X4 matrix for this transformation.
     * @return the transformation matrix.
     */
    mat4 getMatrix();
    /**
     * @brief updateParameter: Update the parameters of this transformation.
     */
    void updateParameter();
    /**
     * @brief updateMatrix: Update the matrix.
     */
    void updateMatrix();
    /**
     * @brief update: Update this transformation.
     */
    void update();
    /* A list of parameters influencing this transformation.*/
    vector<Parameter*> influencingParams;
    /* Add a parameter that influence this transformation. */
    void addParam(Parameter*);
};

#endif // TRANSFORMATION_H
