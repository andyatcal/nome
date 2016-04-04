#include "parameter.h"
#include "mesh.h"
#include "myslider.h"

Parameter::Parameter()
{
    this -> name = QString("");
    influenceMeshes.clear();
    influenceVertices.clear();
    influenceTransformations.clear();
}

float Parameter::getValue()
{
    return value;
}

void Parameter::update()
{
    for(Mesh*& m : influenceMeshes)
    {
        switch(m -> type)
        {
        case 1:
            m -> updateFunnel();
            break;
        case 2:
            m -> updateTunnel();
            break;
        }
    }
    for(Transformation*& t : influenceTransformations)
    {
        t -> update();
    }
}

void Parameter::addInfluenceMesh(Mesh * mesh)
{
    influenceMeshes.push_back(mesh);
}

void Parameter::addInfluenceTransformation(Transformation * t)
{
    influenceTransformations.push_back(t);
}

void Parameter::addInfluenceVertex(Vertex * vertex)
{
    influenceVertices.push_back(vertex);
}

void Parameter::changeParameterValue(float value)
{
    this -> value = value;
    slider -> slider -> setValue(int((this -> value - this -> start) / this -> stepsize + 0.5 ));
}
