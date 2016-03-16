#include "makegroup.h"

void makeGroupTest1(Group &group)
{

}

void makeGroupTest2(Group &group)
{
    Mesh oneMesh;
    makeCube(oneMesh,0.5,0.5,0.5);
    oneMesh.setColor(QColor(255, 255, 0));
    oneMesh.computeNormals();
    mat4 oneTransform = krotate(vec3(1, 1, 1), PI/2);
    oneMesh.setTransformation(oneTransform);
    group.addMesh(oneMesh);
}

void makeGroupTest3(Group &group)
{
    Mesh oneMesh;
    makeCube(oneMesh,0.5,0.5,0.5);
    oneMesh.computeNormals();
    mat4 oneTransform = ktranslate(vec3(1, 1, 1));
    oneMesh.setColor(Qt::green);
    mat4 secondTransform = ktranslate(vec3(-1, -1, -1));
    Mesh secondMesh = meshCopy(oneMesh);
    secondMesh.setColor(Qt::blue);
    secondMesh.computeNormals();
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
    oneMesh.computeNormals();
    Mesh secondMesh = meshCopy(oneMesh);
    oneMesh.setTransformation(oneTransform);
    secondMesh.setTransformation(secondTransform);
    subGroup1.addMesh(oneMesh);
    group.addGroup(subGroup1);
    group.addMesh(secondMesh);
    group.setColor(Qt::white);
    group.assignColor();
}

void makeGroupTest5(Group &group)
{
    Mesh oneMesh;
    Group subGroup1;
    Group subGroup2;
    makeCube(oneMesh,0.5,0.5,0.5);
    oneMesh.computeNormals();
    mat4 oneTransform = ktranslate(vec3(0, 0, 0.5));
    mat4 secondTransform = ktranslate(vec3(0, 0, -0.5));
    Mesh secondMesh = meshCopy(oneMesh);
    oneMesh.setTransformation(oneTransform);
    secondMesh.setTransformation(secondTransform);
    subGroup1.addMesh(oneMesh);
    subGroup2.addMesh(secondMesh);
    group.addGroup(subGroup1);
    group.addGroup(subGroup2);
    group.setColor(Qt::red);
    group.assignColor();
}

void makeGroupTest6(Group &group)
{
    Mesh oneMesh;
    Group subGroup1;
    Group subGroup1_1;
    makeCube(oneMesh,0.5,0.5,0.5);
    oneMesh.computeNormals();
    mat4 oneTransform = ktranslate(vec3(0, 0.5, 0.5));
    mat4 secondTransform = ktranslate(vec3(0, -0.5, -0.5));
    Mesh secondMesh = meshCopy(oneMesh);
    oneMesh.setTransformation(oneTransform);
    secondMesh.setTransformation(secondTransform);
    secondMesh.computeNormals();
    subGroup1.addMesh(oneMesh);
    subGroup1_1.addMesh(secondMesh);
    subGroup1.addGroup(subGroup1_1);
    group.addGroup(subGroup1);
    group.setColor(Qt::blue);
    group.assignColor();
}
