#include "funnel.h"

Funnel::Funnel()
{

}

void Funnel::makeFunnel()
{
    if(n < 3) {
        return;
    }
    vertList.clear();
    edgeTable.clear();
    faceList.clear();
    vector<Vertex*> baseCircle;
    vector<Vertex*> highCircle;
    for(int i = 0; i < n; i++)
    {
        Vertex * newVertex = new Vertex;
        newVertex->ID = i;
        float currAngle = 2.0 * i / n * PI;
        newVertex -> position = vec3(ro * glm::cos(currAngle), ro * glm::sin(currAngle), 0);
        baseCircle.push_back(newVertex);
        addVertex(newVertex);
    }
    float ri = ro * (1 + ratio);
    for(int i = 0; i < n; i++)
    {
        Vertex * newVertex = new Vertex;
        newVertex->ID = i + n;
        float currAngle = 2.0 * i / n * PI;
        newVertex -> position = vec3(ri * glm::cos(currAngle), ri * glm::sin(currAngle), h);
        highCircle.push_back(newVertex);
        addVertex(newVertex);
    }
    for(int i = 0; i < n - 1 ; i++)
    {
        addQuadFace(baseCircle[i], baseCircle[i + 1], highCircle[i + 1], highCircle[i]);
    }
    addQuadFace(baseCircle[n - 1], baseCircle[0], highCircle[0], highCircle[n - 1]);
    buildBoundary();
}
