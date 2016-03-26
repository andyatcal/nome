#include "parameterbank.h"

ParameterBank::ParameterBank()
{
    parameters.clear();
}

void ParameterBank::addParameter(Parameter *param)
{
    parameters.push_back(param);
}

void ParameterBank::setName(QString name)
{
    this -> name = name;
}
