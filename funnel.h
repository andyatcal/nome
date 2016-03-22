#ifndef FUNNEL_H
#define FUNNEL_H

#include "mesh.h"

class Funnel : public Mesh
{
public:
    Funnel();
    int n;
    float ro;
    float ratio;
    float h;
    void makeFunnel();
};

#endif // FUNNEL_H
