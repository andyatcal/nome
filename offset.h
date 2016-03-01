//
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
//
//

#ifndef __OFFSET_H__
#define __OFFSET_H__

#include <vector>
#include "mesh.h"

using namespace glm;

//////////////////////////////////////////////////////////////////////
// Offset Class -- Offset for a given mesh.
//
class Offset{
public:
    // The amount of offset for this mesh.
    float offsetVal;
    // The original mesh.
    Mesh startMesh;
    // The positive offset mesh.
    Mesh posOffsetMesh;
    // The negtive offset mesh.
    Mesh negOffsetMesh;
    // The side offset mesh.
    Mesh sideOffsetMesh;
    // The combination of posOffsetMesh, negOffsetMesh and sideOffsetMesh
    Mesh offsetMesh;
    // Constructor of offset.
    // @param mesh: the initial mesh to find offset
    // @param val: the offsetVal
    Offset(Mesh & mesh, float val);
    // Find the positive and negative offset and their side cover.
    // It runs fast and with sharp edge between the cover and offset surface.
    void makeSeperateOffset();
    // Find the positive, negative and side cover as a whole mesh.
    // Runs slow then the seperate one, but have smooth connection between cover and offset surface.
    void makeFullOffset();
    // Change the value of offset
    // @param val: the offset value, can be positive or negative.
    void changeOffsetValTo(float val);


private:
    // Calculate the offset for one vertex.
    // @param v: the vertex to calculate offset.
    // @param full: true if we are doing a full offset. false if we are doing seperate offset.
    void calcVertexOffset(Vertex * v, bool full);
};

#endif // __OFFSET_H__
