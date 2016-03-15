/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */
#include "group.h"

Group::Group()
{
    subgroups.clear();
    myMeshes.clear();
    myPolylines.clear();
    user_set_color = false;
}

void Group::addMesh(Mesh &mesh)
{
    myMeshes.push_back(mesh);
}

void Group::addGroup(Group &group)
{
    subgroups.push_back(group);
}

void Group::addPolyline(PolyLine &polyline)
{
    myPolylines.push_back(polyline);
}

vector<Mesh> Group::flattenedMeshes()
{
    vector<Mesh> result;
    vector<Mesh>::iterator mIt;
    for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
    {
        transform(*mIt, (*mIt).transformUp);
        result.push_back(*mIt);
    }
    vector<Group>::iterator gIt;
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        vector<Mesh> flattenedFromThisSubGroup = (*gIt).flattenedMeshes();
        for(mIt = flattenedFromThisSubGroup.begin();
            mIt < flattenedFromThisSubGroup.end(); mIt++)
        {
            transform(*mIt, (*gIt).transformUp);
            result.push_back(*mIt);
        }
    }
    return result;
}

vector<PolyLine> Group::flattenedPolylines()
{
    vector<PolyLine> result;
    vector<PolyLine>::iterator pIt;
    for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
    {
        transform(*pIt, (*pIt).transformUp);
        result.push_back(*pIt);
    }
    vector<Group>::iterator gIt;
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        vector<PolyLine> flattenedFromThisSubGroup = (*gIt).flattenedPolylines();
        for(pIt = flattenedFromThisSubGroup.begin();
            pIt < flattenedFromThisSubGroup.end(); pIt++)
        {
            transform(*pIt, (*pIt).transformUp);
            result.push_back(*pIt);
        }
    }
    return result;
}

void Group::assignColor()
{
    vector<Mesh>::iterator mIt;
    vector<PolyLine>::iterator pIt;
    vector<Group>::iterator gIt;
    for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
    {
        if(!((*mIt).user_set_color)) {
            (*mIt).setColor(color);
        }
    }
    for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
    {
        if(!((*pIt).user_set_color)) {
            (*pIt).setColor(color);
        }
    }
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        if(!((*gIt).user_set_color)) {
            (*gIt).setColor(color);
            (*gIt).assignColor();
        }
    }
}

void Group:: setColor(QColor color)
{
    this -> color = color;
}

Mesh Group::merge()
{
    vector<Mesh> flatten = this -> flattenedMeshes();
    return ::merge(flatten);
}

void Group::setTransformation(mat4 new_transformation)
{
    this -> transformUp = new_transformation;
}

void Group::clear()
{
    subgroups.clear();
    myMeshes.clear();
    myPolylines.clear();
}

Group Group::makeCopy()
{
    Group newGroup;
    newGroup.clear();
    vector<Mesh>::iterator mIt;
    vector<Group>::iterator gIt;
    vector<PolyLine>::iterator pIt;
    for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
    {
        Mesh newMesh = meshCopy(*mIt);
        newGroup.addMesh(newMesh);
    }
    for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
    {
        PolyLine newPolyline = polylineCopy(*pIt);
        newGroup.addPolyline(newPolyline);
    }
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        Group copyGroup = (*gIt).makeCopy();
        newGroup.addGroup(copyGroup);
    }
}
