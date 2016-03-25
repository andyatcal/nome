/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#include "transformation.h"

Transformation::Transformation()
{
    int type = 0;
    matrix = mat4(1);
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
    isParametric = false;
    x_expr = "";
    y_expr = "";
    z_expr = "";
    w_expr = "";
}

Transformation::Transformation(int type, float x, float y, float z)
{
    this -> type = type;
    this -> x = x;
    this -> y = y;
    this -> z = z;
    isParametric = false;
    x_expr = "";
    y_expr = "";
    z_expr = "";
    w_expr = "";
    update();
}

Transformation::Transformation(int type, float x, float y, float z, float w)
{
    this -> type = type;
    this -> x = x;
    this -> y = y;
    this -> z = z;
    this -> w = w;
    isParametric = false;
    x_expr = "";
    y_expr = "";
    z_expr = "";
    w_expr = "";
    update();
}

Transformation::Transformation(string input)
{

}

void Transformation::rotate()
{
    matrix = glm::rotate(w, vec3(x, y, z));
}

void Transformation::scale()
{
    matrix = glm::scale(vec3(x, y, z));
}

void Transformation::translate()
{
    matrix = glm::translate(vec3(x, y, z));
}

void Transformation::mirror()
{
    float d = w;
    vec3 plane_normal = vec3(x, y, z);
    float k = - d / length(plane_normal);
    plane_normal = normalize(plane_normal);
    float a = plane_normal[0];
    float b = plane_normal[1];
    float c = plane_normal[2];
    matrix[0][0] = 1 - 2 * a * a;
    matrix[0][1] = - 2 * a * b;
    matrix[0][2] = - 2 * a * c;
    matrix[0][3] = 0;
    matrix[1][0] = - 2 * a * b;
    matrix[1][1] = 1 - 2 * b * b;
    matrix[1][2] = - 2 * b * c;
    matrix[1][3] = 0;
    matrix[2][0] = - 2 * a * c;
    matrix[2][1] = - 2 * b * c;
    matrix[2][2] = 1 - 2 * c * c;
    matrix[2][3] = 0;
    matrix[3][0] = 2 * a * k;
    matrix[3][1] = 2 * b * k;
    matrix[3][2] = 2 * c * k;
    matrix[3][3] = 1;
}

mat4 Transformation::getMatrix()
{
    return matrix;
}

void Transformation::update()
{
    switch (type) {
    case 1:
        rotate();
        break;
    case 2:
        scale();
        break;
    case 3:
        translate();
        break;
    case 4:
        mirror();
        break;
    }
}
