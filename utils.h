#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include "parameter.h"
#include <stack>
#include <vector>

using namespace glm;

float evaluate_expression(string expr, unordered_map<string, Parameter> *params);

float getParameterValue(string name, unordered_map<string, Parameter> *params);

bool isOperator(char token);

float getVal(float x, float y, char oper);

int getPriority(char oper);

vec3 getXYZ(string input);

float getOneValue(string input);

vec4 getXYZW(string input);

#endif // UTILS_H
