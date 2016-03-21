#ifndef PARAMETERBANK_H
#define PARAMETERBANK_H

#include <vector>
#include "parameter.h"

class ParameterBank
{
public:
    ParameterBank();
    vector<Parameter> parameters;
    void addParameter(Parameter param);
    void setName(QString name);
    QString name;
};

#endif // PARAMETERBANK_H
