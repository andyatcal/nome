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
    for(Vertex*& v : vertices)
    {
        delete v;
    }
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

void PolyLine::drawLineWithCubes(int start_index)
{
    int counter = 0;
    updateCubeSizes();
    for(Vertex*& v : vertices)
    {
        glLoadName(start_index + counter);
        drawCubeAroundVertex(v, 0.1 * cubeSizes[counter]);
        counter++;
    }
    drawLine(start_index);
}

void PolyLine::drawCubeAroundVertex(Vertex *v, float size)
{
    vec3 p = v -> position;
    float x = p[0];
    float y = p[1];
    float z = p[2];
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glVertex3f(x + size / 2, y + size / 2, z + size / 2);
        glVertex3f(x - size / 2, y + size / 2, z + size / 2);
        glVertex3f(x - size / 2, y - size / 2, z + size / 2);
        glVertex3f(x + size / 2, y - size / 2, z + size / 2);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glVertex3f(x + size / 2, y + size / 2, z + size / 2);
        glVertex3f(x + size / 2, y - size / 2, z + size / 2);
        glVertex3f(x + size / 2, y - size / 2, z - size / 2);
        glVertex3f(x + size / 2, y + size / 2, z - size / 2);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glVertex3f(x + size / 2, y + size / 2, z - size / 2);
        glVertex3f(x + size / 2, y - size / 2, z - size / 2);
        glVertex3f(x - size / 2, y - size / 2, z - size / 2);
        glVertex3f(x - size / 2, y + size / 2, z - size / 2);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glVertex3f(x - size / 2, y + size / 2, z - size / 2);
        glVertex3f(x - size / 2, y - size / 2, z - size / 2);
        glVertex3f(x - size / 2, y - size / 2, z + size / 2);
        glVertex3f(x - size / 2, y + size / 2, z + size / 2);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(x + size / 2, y + size / 2, z - size / 2);
        glVertex3f(x + size / 2, y + size / 2, z + size / 2);
        glVertex3f(x - size / 2, y + size / 2, z + size / 2);
        glVertex3f(x - size / 2, y + size / 2, z - size / 2);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        glVertex3f(x + size / 2, y - size / 2, z + size / 2);
        glVertex3f(x + size / 2, y - size / 2, z - size / 2);
        glVertex3f(x - size / 2, y - size / 2, z - size / 2);
        glVertex3f(x - size / 2, y - size / 2, z + size / 2);
    glEnd();
}

void PolyLine::updateCubeSizes()
{
    cubeSizes.clear();
    vector<Vertex*>::iterator vIt;
    vector<float> distances;
    for(vIt = vertices.begin(); vIt < vertices.end() - 1; vIt++)
    {
        distances.push_back(glm::distance((*vIt) -> position, (*(vIt + 1)) -> position));
    }
    if(isLoop)
    {
        distances.push_back(glm::distance((*vIt) -> position, vertices[0] -> position));
        for(size_t i = 0; i < distances.size(); i++)
        {
            if(i == 0)
            {
                cubeSizes.push_back((distances[0] + *(distances.end() - 1)) / 2);
            }
            else
            {
                cubeSizes.push_back((distances[i] + distances[i - 1]) / 2);
            }
        }
    }
    else
    {
        for(size_t i = 0; i <= distances.size(); i++)
        {
            if(i == 0)
            {
                cubeSizes.push_back(distances[0]);
            }
            else if(i == distances.size())
            {
                cubeSizes.push_back(distances[distances.size() - 1]);
            }
            else
            {
                cubeSizes.push_back((distances[i - 1] + distances[i]) / 2);
            }
        }
    }
}

void PolyLine::drawVertices()
{
    glPointSize(10);
    glBegin(GL_POINTS);
    for(Vertex*& v : vertices)
    {
        if(v -> selected)
        {
            vec3 p = v -> position;
            glNormal3f(p[0] * 100, p[1] * 100, p[2] * 100);
            glVertex3f(p[0], p[1], p[2]);
        }
    }
    glEnd();
}

Vertex * PolyLine::findVertexInThisPolyline(string name)
{
    for(Vertex*& v: vertices)
    {
        if(v->name == name)
        {
            return v;
        }
    }
    return NULL;
}
