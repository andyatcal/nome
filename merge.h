/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef __MERGE_H__
#define __MERGE_H__

#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
#include "mesh.h"

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Merge Class -- A merge class contains fuctions to merge meshes.

// Return true if two vertices are very close to each other.
bool vertexMatch(Vertex * v1, Vertex * v2);

// Merge any possible boundary edges that are close for two meshes.
// @param mesh1, mesh2. The two meshes to be merged.
// Return a new mesh that contains the merged mesh.
Mesh merge(Mesh & mesh1, Mesh & mesh2);

// Merge any possible boundary edges that are close for multiple meshes.
// @param meshes. The list of meshes to be merged.
// All facets and vertices of mesh2 will be added to mesh1.
// Return a new mesh that contains the merged mesh.
Mesh merge(vector<Mesh> &meshes);

#endif // __MESH_H__
