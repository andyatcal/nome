#ifndef FUNNEL_H
#define FUNNEL_H

#include "mesh.h"
#include "utils.h"
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
    void updateFunnel_n();
    void updateFunnel_ro();
    void updateFunnel_ratio_or_h();
    void setParameterValues(string);
};

#endif // FUNNEL_H
