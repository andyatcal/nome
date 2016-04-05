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

void PolyLine::drawLine(int start_index)
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

void PolyLine::transform(Transformation *t)
{
    mat4 matrix = t -> getMatrix();
    vector<Vertex*>::iterator vIt;
    for(vIt = vertices.begin(); vIt < vertices.end(); vIt++) {
        (*vIt) -> position = vec3(matrix * vec4((*vIt) -> position, 1));
    }
}

PolyLine PolyLine::makeCopy(string copy_polyline_name)
{
    PolyLine newPolyline;
    newPolyline.clear();
    if(copy_polyline_name == "")
    {
        newPolyline.name = this -> name;
    }
    else
    {
        newPolyline.name = copy_polyline_name;
    }
    newPolyline.isLoop = this->isLoop;
    newPolyline.color = color;
    for(Vertex*& v: vertices)
    {
        Vertex* newVertex = new Vertex;
        newVertex -> ID = v -> ID;
        newVertex -> name = v -> name;
        newVertex -> position = v -> position;
        newVertex -> isParametric = v -> isParametric;
        if(v -> isParametric)
        {
            newVertex -> x_expr = v -> x_expr;
            newVertex -> y_expr = v -> y_expr;
            newVertex -> z_expr = v -> z_expr;
            newVertex -> influencingParams = v -> influencingParams;
            newVertex -> params = v -> params;
        }
        newPolyline.addVertex(newVertex);
    }
    return newPolyline;
}

PolyLine PolyLine::makeCopyForTransform()
{
    PolyLine newPolyline;
    newPolyline.before_transform_polyline = this;
    newPolyline.name = this -> name;
    newPolyline.isLoop = this->isLoop;
    newPolyline.color = color;
    for(Vertex*& v: vertices)
    {
        Vertex* newVertex = new Vertex;
        newVertex -> ID = v -> ID;
        newVertex -> name = v -> name;
        newVertex -> position = v -> position;
        newVertex -> before_transform_vertex = v;
        newPolyline.addVertex(newVertex);
    }
    return newPolyline;
}

void PolyLine::updateCopyForTransform()
{
    transformations_up = before_transform_polyline -> transformations_up;
    for(Vertex*& v: vertices)
    {
        v -> position = v -> before_transform_vertex -> position;
    }
}
