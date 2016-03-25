#ifndef PARAMETER_H
#define PARAMETER_H

#include <QString>
#include <vector>
class Funnel;
class Vertex;
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
    /* A list of objects to update when changing value of this parameter. */
    vector<Vertex*> influenceVertices;
    vector<Funnel*> influenceFunnels;
    void update();
};

#endif // PARAMETER_H
