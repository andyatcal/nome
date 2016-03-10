//
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
//
//

#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <QMainWindow>
#include <QtOpenGL>
#if __linux__
 #include <GL/glut.h>
 #include <GL/gl.h>
#elif __APPLE__
 #include <GLUT/GLUT.h>
#endif
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#endif

#include "face.h"

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Mesh Class -- A MESH is a collection of polygon facets
class Mesh{
public:
    // A list of all vertices in this mesh.
    vector<Vertex*> vertList;
    // A list of all facets in this mesh.
    vector<Face*> faceList;
    // Transfromation matrix of the object (mesh).
    mat4 object2world;
    // This is an auxillary table to build a mesh, matching edge to vertex.
    unordered_map<Vertex*, vector<Edge*> > edgeTable;
    // Constructor.
    Mesh();
    // Add one vertex to this Mesh.
    // @param v: the vertex to be added in.
    void addVertex(Vertex * v);
    // Add one edge v1-v2 to this Mesh.
    // @param v1, v2: the two vertices of this edge.
    // If it already exists, then return the existing edge.
    Edge * createEdge(Vertex * v1, Vertex * v2);
    // Find one edge v1-v2 in this Mesh.
    // @param v1, v2: the two vertices of this edge.
    // If it does not exists, then return NULL.
    Edge * findEdge(Vertex * v1, Vertex * v2);
    // Add a triangle face to a mesh, with three vertices.
    // @param v1, v2, v3 are the three vertices of the face.
    void addTriFace(Vertex * v1, Vertex * v2, Vertex * v3);
    // Add a quad face to a mesh, with three vertices.
    // @param v1, v2, v3, v4 are the four vertices of the face.
    void addQuadFace(Vertex * v1, Vertex * v2, Vertex * v3, Vertex * v4);
    // Add a arbitrary polygon face to a mesh, with three vertices.
    // @param vertices is a list of consequtive vertices of the face.
    void addPolygonFace(vector<Vertex*> vertices, bool reverseOrder = false);
    // Draw mesh in OpenGL
    void drawMesh();
    // Draw the selected vertices in OpenGL
    void drawVertices();
    // Build Boundary Pointers for Mesh.
    void buildBoundary();
    // Compute the vertex normals for every face and vertex of the mesh.
    void computeNormals();
    // Return the list of edges on the boarders
    vector<Edge*> boundaryEdgeList();
    // The color of this mesh
    vec3 color;
};

// @param p1, p2, p3 are positions of three vertices,
// with edge p1 -> p2 and edge p2 -> p3.
vec3 getNormal3Vertex(vec3 p1, vec3 p2, vec3 p3);

static GLfloat WHITE[] = {0.8f, 0.8f, 0.8f, 1.0f};
static GLfloat RED[] = {0.8f, 0.0f, 0.0f, 1.0f};
static GLfloat GREEN[] = {0.0f, 0.8f, 0.0f, 1.0f};
static GLfloat BLUE[] = {0.0f, 0.0f, 0.8f, 1.0f};
static GLfloat YELLOW[] = {.8f, .8f, 0.f, 1.f};
static GLfloat PURPLE[] = {.8f, 0.f, .8f, 1.f};
static GLfloat CYAN[] = {.0f, .8f, 0.8f, 1.f};

#define PI (glm::pi<float>())
#define VERYSMALLVALUE 0.001

#endif // __MESH_H__
