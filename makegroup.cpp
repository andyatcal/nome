#include "makegroup.h"

void makeGroupTest1(Group &group)
{

}

void makeGroupTest2(Group &group)
{
    Mesh oneMesh;
    makeCube(oneMesh,0.5,0.5,0.5);
    mat4 oneTransform = krotate(vec3(1, 1, 1), PI/2);
    oneMesh.setTransformation(oneTransform);
    group.addMesh(oneMesh);
}

void makeGroupTest3(Group &group)
{
    Mesh oneMesh;
    makeCube(oneMesh,0.5,0.5,0.5);
    mat4 oneTransform = ktranslate(vec3(1, 1, 1));
    mat4 secondTransform = ktranslate(vec3(-1, -1, -1));
    Mesh secondMesh = meshCopy(oneMesh);
    oneMesh.setTransformation(oneTransform);
    secondMesh.setTransformation(secondTransform);
    group.addMesh(oneMesh);
    group.addMesh(secondMesh);
}

void makeGroupTest4(Group &group)
{
    Mesh oneMesh;
    Group subGroup1;
    makeCube(oneMesh,0.5,0.5,0.5);
    mat4 oneTransform = ktranslate(vec3(1, 1, 1));
    mat4 secondTransform = ktranslate(vec3(-1, -1, -1));
    Mesh secondMesh = meshCopy(oneMesh);
    oneMesh.setTransformation(oneTransform);
    secondMesh.setTransformation(secondTransform);
    subGroup1.addMesh(oneMesh);
    group.addGroup(subGroup1);
    group.addMesh(secondMesh);
}

void makeGroupTest5(Group &group)
{
    Mesh oneMesh;
    Group subGroup1;
    Group subGroup2;
    makeCube(oneMesh,0.5,0.5,0.5);
    mat4 oneTransform = ktranslate(vec3(1, 1, 1));
    mat4 secondTransform = ktranslate(vec3(-1, -1, -1));
    Mesh secondMesh = meshCopy(oneMesh);
    oneMesh.setTransformation(oneTransform);
    secondMesh.setTransformation(secondTransform);
    subGroup1.addMesh(oneMesh);
    subGroup2.addMesh(secondMesh);
    group.addGroup(subGroup1);
    group.addGroup(subGroup2);
}

void makeGroupTest6(Group &group)
{

}
