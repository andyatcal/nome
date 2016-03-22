/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef __MAKEMESH_H__
#define __MAKEMESH_H__

#include <regex>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>
#include "mesh.h"
#include "merge.h"
#include "transformation.h"

using namespace glm;
using namespace std;

//////////////////////////////////////////////////////////////////////
// MakeMesh Class -- Create Initial Meshes.
void makeSquare(Mesh &mesh);

Mesh mergeTwoMeshes1();

Mesh mergeTwoMeshes2();

Mesh mergeTwoMeshes3();

Mesh mergeTwoMeshes8();

Mesh mergeTwoMeshes4();

Mesh mergeTwoMeshes7();

Mesh mergeTwoMeshes6();

Mesh mergeTwoMeshes5();

Mesh mirrorTest();

void makePyramid(Mesh &mesh);

void makeCube(Mesh &mesh);

void makeCube(Mesh &mesh, float x, float y, float z);

void makeOpenCube(Mesh &mesh);

void makeRing(Mesh &mesh);

void makeSharpCube(Mesh &mesh);

void makeOctahedron(Mesh &mesh);

void makeSharpOctahedron(Mesh &mesh);

void makeNormalStrip(Mesh &mesh);

void makeMobius(Mesh &mesh);

void makeCircleSweep(Mesh &mesh);

void makeWithSIF(Mesh &mesh, string inputSIF);

void makeWithQuadSIF(Mesh &mesh, string inputSIF);

#endif // __MAKEMESH_H__
