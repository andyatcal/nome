//
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
//
//

#ifndef __SUBDIVISION_H__
#define __SUBDIVISION_H__

#include "mesh.h"
#include "merge.h"
#include "makeMesh.h"

using namespace glm;
using namespace std;

//////////////////////////////////////////////////////////////////////
// Subdivision Class -- Functions to perform the subdivision for a mesh
class Subdivision{
public:
    // The mesh to be subdivide;
    Mesh currMesh;
    // Constructor.
    Subdivision(){};
    // Constructor.
    // @param mesh: the reference of given mesh.
    Subdivision(Mesh & mesh);
    // The integration of subdivision.
    // @param level: level of Catmull-Clark subdivision.
    Mesh ccSubdivision(int level);
private:
    // Construct face points in Catmull-Clark subdivision.
    // Computed values are stored in face.facepoint. Add new Vertices to vertTable.
    // @param newVertList: The list of vertices for next level mesh.
    void makeFacePoints(vector<Vertex*> &newVertList);
    // Construct edge points in Catmull-Clark subdivision.
    // Computed values are stored in edge.edgepoint. Add new Vertices to vertTable.
    // @param newVertList: The list of vertices for next level mesh.
    void makeEdgePoints(vector<Vertex*> &newVertList);
    // Construct vertex points in Catmull-Clark subdivision with DeRose et al's Equation.
    // By modifying the original position and pointers of the vertex.
    // @param newVertList: The list of vertices for next level mesh.
    void makeVertexPointsD(vector<Vertex*> &newVertList);
    // Similar to the above function, but with Catmull-Clark's equation.
    // @param newVertList: The list of vertices for next level mesh.
    void makeVertexPointsC(vector<Vertex*> &newVertList);
    // Construct a new mesh after given new facepoints, edgepoints, and vertexpoints.
    // @param newFaceList: the list of faces for the next level mesh.
    void compileNewMesh(vector<Face*> &newFaceList);
};

#endif // __SUBDIVISION_H__
