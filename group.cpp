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
    parent = NULL;
    in_editing_mode = false;
}

void Group::addMesh(Mesh &mesh)
{
    myMeshes.push_back(mesh);
    mesh.parent = this;
}

void Group::addGroup(Group &group)
{
    subgroups.push_back(group);
    group.parent = this;
}

void Group::addPolyline(PolyLine &polyline)
{
    myPolylines.push_back(polyline);
    polyline.parent = this;
}

vector<Mesh*> Group::flattenedMeshes()
{
    vector<Mesh*> result;
    vector<Mesh>::iterator mIt;
    for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
    {
        for(Transformation& transformUp : (*mIt).transformations_up)
        {
            (*mIt).transform(&transformUp);
        }
        (*mIt).computeNormals();
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
            for(Transformation& transformUp : (*gIt).transformations_up)
            {
                (**mpIt).transform(&transformUp);
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
        for(Transformation& transformUp : (*pIt).transformations_up)
        {
            (*pIt).transform(&transformUp);
        }
        result.push_back(&(*pIt));
    }
    vector<Group>::iterator gIt;
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        vector<PolyLine*> flattenedFromThisSubGroup = (*gIt).flattenedPolylines();
        vector<PolyLine*>::iterator ppIt;
        for(ppIt = flattenedFromThisSubGroup.begin();
            ppIt < flattenedFromThisSubGroup.end(); ppIt++)
        {
            for(Transformation& transformUp : (*gIt).transformations_up)
            {
               (*(*ppIt)).transform(&transformUp);
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
        if(!((*mIt).user_set_color))
        {
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
        if(!((*gIt).user_set_color))
        {
            (*gIt).setColor(color);
            (*gIt).assignColor();
        }
        else
        {
            (*gIt).assignColor();
        }
    }
}

void Group::updateGroupElementName()
{
    vector<Mesh>::iterator mIt;
    vector<PolyLine>::iterator pIt;
    vector<Group>::iterator gIt;
    if(this -> name != "" || this -> parent == NULL)
    {
        for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
        {
            if(this -> parent != NULL)
            {
                (*mIt).name = this->name + "_" + (*mIt).name;
            }
            for(Vertex*& v : (*mIt).vertList)
            {
                v -> name = (*mIt).name + "_" + v -> name;
            }
            for(Face*& f : (*mIt).faceList)
            {
                if(f -> name == "")
                {
                    f -> name = (*mIt).name + "_" + to_string(f -> id);
                }
                else
                {
                    f -> name = (*mIt).name + "_" + f -> name;
                }
            }
        }
        for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
        {
            (*pIt).name = this -> name + "_" + (*pIt).name;
            for(Vertex*& v: (*pIt).vertices)
            {
                v -> name = (*pIt).name + "_" + v -> name;
            }
        }
    }
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        (*gIt).updateGroupElementName();
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

void Group::addTransformation(Transformation new_transform)
{
    transformations_up.push_back(new_transform);
}

void Group::setTransformation(vector<Transformation> new_transforms)
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

Group Group::makeCopy(string copy_group_name)
{
    Group newGroup;
    newGroup.clear();
    if(copy_group_name == "")
    {
        newGroup.name = this->name;
    }
    else
    {
        newGroup.name = copy_group_name;
    }
    vector<Mesh>::iterator mIt;
    vector<Group>::iterator gIt;
    vector<PolyLine>::iterator pIt;
    for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
    {
        Mesh newMesh = (*mIt).makeCopy();
        newMesh.transformations_up = (*mIt).transformations_up;
        newMesh.user_set_color = (*mIt).user_set_color;
        newMesh.color = (*mIt).color;
        newGroup.addMesh(newMesh);
    }
    for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
    {
        PolyLine newPolyline = (*pIt).makeCopy();
        newPolyline.transformations_up = (*pIt).transformations_up;
        newPolyline.user_set_color = (*pIt).user_set_color;
        newPolyline.color = (*pIt).color;
        newGroup.addPolyline(newPolyline);
    }
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        Group copyGroup = (*gIt).makeCopy();
        copyGroup.transformations_up = (*gIt).transformations_up;
        copyGroup.user_set_color = (*gIt).user_set_color;
        copyGroup.color = (*gIt).color;
        newGroup.addGroup(copyGroup);
    }
    return newGroup;
}

Group Group::makeCopyForTransform()
{
    Group newGroup;
    newGroup.before_transform_group = this;
    newGroup.name = this->name;
    newGroup.clear();
    vector<Mesh>::iterator mIt;
    vector<Group>::iterator gIt;
    vector<PolyLine>::iterator pIt;
    for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
    {
        Mesh newMesh = (*mIt).makeCopyForTransform();
        newMesh.transformations_up = (*mIt).transformations_up;
        newMesh.user_set_color = (*mIt).user_set_color;
        newMesh.color = (*mIt).color;
        newGroup.addMesh(newMesh);
    }
    for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
    {
        PolyLine newPolyline = (*pIt).makeCopyForTransform();
        newPolyline.transformations_up = (*pIt).transformations_up;
        newPolyline.user_set_color = (*pIt).user_set_color;
        newPolyline.color = (*pIt).color;
        newGroup.addPolyline(newPolyline);
    }
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        Group copyGroup = (*gIt).makeCopyForTransform();
        copyGroup.transformations_up = (*gIt).transformations_up;
        copyGroup.user_set_color = (*gIt).user_set_color;
        copyGroup.color = (*gIt).color;
        newGroup.addGroup(copyGroup);
    }
    return newGroup;
}

void Group::updateCopyForTransform()
{
    transformations_up = before_transform_group -> transformations_up;
    vector<Mesh>::iterator mIt;
    vector<Group>::iterator gIt;
    vector<PolyLine>::iterator pIt;
    for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
    {
        (*mIt).updateCopyForTransform();
    }
    for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
    {
        (*pIt).updateCopyForTransform();
    }
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        (*gIt).updateCopyForTransform();
    }
}
void Group::setName(string name)
{
    this -> name = name;
}

void Group::mapFromParameters()
{
    bool found;
    for(Transformation& t : transformations_up)
    {
        vector<Parameter*> params = t.influencingParams;
        for(Parameter*& p : params)
        {
            p->addInfluenceTransformation(&t);
        }
    }
    vector<Mesh>::iterator mIt;
    for(mIt = myMeshes.begin(); mIt < myMeshes.end(); mIt++)
    {
        vector<Parameter*> params = (*mIt).influencingParams;
        for(Parameter*& p : params)
        {
            p->addInfluenceMesh(&(*mIt));
        }
        for(Transformation& t : (*mIt).transformations_up)
        {
            vector<Parameter*> params = t.influencingParams;
            for(Parameter*& p : params)
            {
                p->addInfluenceTransformation(&t);
            }
        }
        for(Vertex*& v : (*mIt).vertList)
        {
            vector<Parameter*> params = v -> influencingParams;
            for(Parameter*& p : params)
            {
                found = false;
                for(Vertex * vin : p -> influenceVertices)
                {
                    if(vin == v)
                    {
                        found = true;
                    }
                }
                if(!found)
                {
                    p->addInfluenceVertex(v);
                }
            }
        }
    }
    vector<PolyLine>::iterator pIt;
    for(pIt = myPolylines.begin(); pIt < myPolylines.end(); pIt++)
    {
        for(Vertex*& v : (*pIt).vertices)
        {
            vector<Parameter*> params = v -> influencingParams;
            for(Parameter*& p : params)
            {
                found = false;
                for(Vertex * vin : p -> influenceVertices)
                {
                    if(vin == v)
                    {
                        found = true;
                    }
                }
                if(!found)
                {
                    p->addInfluenceVertex(v);
                }
            }
        }
        for(Transformation& t : (*pIt).transformations_up)
        {
            vector<Parameter*> params = t.influencingParams;
            for(Parameter*& p : params)
            {
                p->addInfluenceTransformation(&t);
            }
        }
    }
    vector<Group>::iterator gIt;
    for(gIt = subgroups.begin(); gIt < subgroups.end(); gIt++)
    {
        (*gIt).mapFromParameters();
    }
}

Vertex* Group::findVertexInThisGroup(string name)
{
    for(Mesh &mesh : myMeshes)
    {
        Vertex *v = mesh.findVertexInThisMesh(name);
        if(v != NULL)
        {
            return v;
        }
    }
    for(PolyLine &polyline : myPolylines)
    {
        Vertex *v = polyline.findVertexInThisPolyline(name);
        if(v != NULL)
        {
            return v;
        }
    }
    for(Group& group : subgroups)
    {
        Vertex *v = group.findVertexInThisGroup(name);
        if(v != NULL)
        {
            return v;
        }
    }
    return NULL;
}

bool Group::deleteFaceInThisGroup(string name)
{
    for(Mesh &mesh : myMeshes)
    {
        bool found = mesh.deleteFaceInThisMesh(name);
        if(found)
        {
            return true;
        }
    }
    for(Group& group : subgroups)
    {
        bool found = group.deleteFaceInThisGroup(name);
        if(found)
        {
            return true;
        }
    }
    return false;
}
