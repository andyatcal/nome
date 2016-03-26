#include "parameter.h"
#include "mesh.h"

Parameter::Parameter()
{
    this -> name = QString("");
    influenceMeshes.clear();
    influenceVertices.clear();
}

float Parameter::getValue()
{
    return value;
}

void Parameter::update()
{
    for(Mesh*& m : influenceMeshes)
    {
        if(m -> type == 1)
        {
            m -> updateFunnel();
        }
    }
}

void Parameter::addInfluenceMesh(Mesh * mesh)
{
    influenceMeshes.push_back(mesh);
}

void Parameter::addInfluenceVertex(Vertex * vertex)
{
    influenceVertices.push_back(vertex);
}
