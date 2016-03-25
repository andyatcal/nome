#ifndef PARAMETER_H
#define PARAMETER_H

#include <QString>
using namespace std;

/**
 * @brief The Parameter class. The parameter to control the shape.
 * Every parameter will be controled by a slider bar.
 */
class Parameter
{
public:
    Parameter();
    QString name;
    float start;
    float end;
    float stepsize;
    float value;
    float getValue();
};

#endif // PARAMETER_H
