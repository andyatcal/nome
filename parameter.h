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
    int start;
    int end;
    int stepsize;
    int value;
    int multiplier;
};

#endif // PARAMETER_H
