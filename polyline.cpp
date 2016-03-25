/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#include "polyline.h"

PolyLine::PolyLine()
{
    isLoop = false;
    vertices.clear();
    user_set_color = false;
    transformations_up.clear();
}

void PolyLine::drawLine()
{
    if(!isLoop)
    {
        glBegin(GL_LINE_STRIP);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }
    vector<Vertex*>::iterator vIt;
    for(vIt = vertices.begin(); vIt < vertices.end(); vIt ++)
    {
        vec3 position = (*vIt) -> position;
        glVertex3f(position[0], position[1], position[2]);
    }
    glEnd();
}

void PolyLine::clear()
{
    vertices.clear();
    transformations_up.clear();
}

bool PolyLine::isEmpty()
{
    return vertices.size() == 0;
}

vector<float> PolyLine::skewness()
{
    if(vertices.size() < 2) {
        cout<<"ERROR: Can not find the skewness of a polyline with one vertex."<<endl;
        exit(0);
    }
    vector<float> distances;
    distances.push_back(0);
    vector<Vertex*>::iterator vIt;
    float sum = 0;
    for(vIt = vertices.begin(); vIt < vertices.end() - 1; vIt++) {
        float currDistance = distance((*(vIt + 1)) -> position, (*vIt) -> position);
        sum += currDistance;
        distances.push_back(sum);
    }
    if(isLoop) {
        sum += distance(vertices[0] -> position, (*vIt) -> position);
    }
    vector<float>::iterator dIt;
    for(dIt = distances.begin(); dIt < distances.end(); dIt++) {
        (*dIt) /= sum;
        cout<<(*dIt)<<endl;
    }
    return distances;
}

void PolyLine::setColor(QColor color)
{
    this -> color = color;
}

void PolyLine::addTransformation(Transformation new_transform)
{
    transformations_up.push_back(new_transform);
}

void PolyLine::setTransformation(vector<Transformation> new_transform)
{
    transformations_up = new_transform;
}

void PolyLine::addVertex(Vertex *v)
{
    vertices.push_back(v);
}

PolyLine PolyLine::makeCopy()
{
    PolyLine newPolyline;
    newPolyline.clear();
    vector<Vertex*>::iterator vIt;
    for(vIt = vertices.begin();
        vIt < vertices.end(); vIt ++) {
        Vertex * vertCopy = new Vertex;
        vertCopy -> ID = (*vIt) -> ID;
        vertCopy -> position = (*vIt) -> position;
        newPolyline.addVertex(vertCopy);
    }
    newPolyline.setColor(color);
    return newPolyline;
}

void PolyLine::transform(Transformation *t)
{
    mat4 matrix = t -> getMatrix();
    vector<Vertex*>::iterator vIt;
    for(vIt = vertices.begin(); vIt < vertices.end(); vIt++) {
        (*vIt) -> position = vec3(matrix * vec4((*vIt) -> position, 1));
    }
}
