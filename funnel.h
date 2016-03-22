#ifndef FUNNEL_H
#define FUNNEL_H

#include "mesh.h"
#include <string>

class Funnel : public Mesh
{
public:
    Funnel();
    int n;
    float ro;
    float ratio;
    float h;
    string n_expr;
    string ro_expr;
    string ratio_expr;
    string h_expr;
    void makeFunnel();
    void updateFunnel();
    void setParameterValues(string);
};

#endif // FUNNEL_H
