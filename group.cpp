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
    transformations_up.clear();
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

vector<Mesh*> Group::flattenedMeshes()
{
    vector<Mesh*> result;
    vector<Mesh>::iterator mIt;
    for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
    {
        for(mat4& transformUp : (*mIt).transformations_up)
        {
            transform(*mIt, transformUp);
        }
        result.push_back(&(*mIt));
    }
    vector<Group>::iterator gIt;
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        vector<Mesh*> flattenedFromThisSubGroup = (*gIt).flattenedMeshes();
        vector<Mesh*>::iterator mpIt;
        for(mpIt = flattenedFromThisSubGroup.begin();
            mpIt < flattenedFromThisSubGroup.end(); mpIt++)
        {
            for(mat4& transformUp : (*gIt).transformations_up)
            {
                transform(*(*mpIt), transformUp);
            }
            result.push_back(*mpIt);
        }
    }
    return result;
}

vector<PolyLine*> Group::flattenedPolylines()
{
    vector<PolyLine*> result;
    vector<PolyLine>::iterator pIt;
    for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
    {
        for(mat4& transformUp : (*pIt).transformations_up)
        {
            transform(*pIt, transformUp);
            result.push_back(&(*pIt));
        }
    }
    vector<Group>::iterator gIt;
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        vector<PolyLine*> flattenedFromThisSubGroup = (*gIt).flattenedPolylines();
        vector<PolyLine*>::iterator ppIt;
        for(ppIt = flattenedFromThisSubGroup.begin();
            ppIt < flattenedFromThisSubGroup.end(); ppIt++)
        {
            for(mat4& transformUp : (*gIt).transformations_up)
            {
                transform(*(*ppIt), transformUp);
            }
            result.push_back(*ppIt);
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
    vector<Mesh*> flatten = this -> flattenedMeshes();
    return ::merge(flatten);
}

void Group::addTransformation(mat4 new_transform)
{
    transformations_up.push_back(new_transform);
}

void Group::setTransformation(vector<mat4> new_transforms)
{
    transformations_up = new_transforms;
}

void Group::clear()
{
    subgroups.clear();
    myMeshes.clear();
    myPolylines.clear();
    transformations_up.clear();
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
        newMesh.transformations_up = (*mIt).transformations_up;
        newGroup.addMesh(newMesh);
    }
    for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
    {
        PolyLine newPolyline = polylineCopy(*pIt);
        newPolyline.transformations_up = (*pIt).transformations_up;
        newGroup.addPolyline(newPolyline);
    }
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        Group copyGroup = (*gIt).makeCopy();
        copyGroup.transformations_up = (*gIt).transformations_up;
        newGroup.addGroup(copyGroup);
    }
    return newGroup;
}

void Group::setName(string name)
{
    this -> name = name;
}
