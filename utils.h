#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <unordered_map>
#include "parameter.h"

float evaluate_expression(string expr, unordered_map<string, Parameter> *params);

float getParameterValue(string name, unordered_map<string, Parameter> *params);

#endif // UTILS_H
