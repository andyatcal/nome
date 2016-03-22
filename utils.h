#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <unordered_map>
#include "parameter.h"
#include <stack>
#include <vector>

float evaluate_expression(string expr, unordered_map<string, Parameter> *params);

float getParameterValue(string name, unordered_map<string, Parameter> *params);

bool isOperator(char token);

float getVal(float x, float y, char oper);

int getPriority(char oper);

#endif // UTILS_H
