#include "parameter.h"

Parameter::Parameter()
{
    this -> name = QString("");
}

float Parameter::getValue()
{
    return this -> value * this -> multiplier;
}
