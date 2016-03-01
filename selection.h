//
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
//
//

#ifndef __SELECTION_H__
#define __SELECTION_H__

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>
#include "polyline.h"
#include "mesh.h"

using namespace glm;
using namespace std;

//////////////////////////////////////////////////////////////////////
// Selection Class -- Handles Mouse Selection.

class Selection {
    // Vertices selected. Can be used to create Face or Polyline.
    vector<Vertex*> selectedVertices;
    // For partial border selection
    Vertex * firstBorderSelectionPoint = NULL;
    Vertex * secondBorderSelectionPoint = NULL;
    vector<Vertex*> allBorderPoints;
    vector<Vertex*> firstHalfBorder;
    vector<Vertex*> vertToSelect;
    // Debug function
    void list_hits(GLint hits, GLuint *names);

    // Select the hitted face from mesh.
    // @param mesh, the mesh that contains this face.
    void selectFace(Mesh & mesh, GLint hits, GLuint *names);
    // Select the nearst point.
    // @param mesh, the mesh that contains this vertex.
    void selectVertex(Mesh & mesh, GLint hits, GLuint *names,
                      GLdouble posX, GLdouble posY, GLdouble posZ);
    // Select all border vertices
    // @param mesh, the mesh that contains this border
    void selectWholeBorder(Mesh & mesh, GLint hits, GLuint *names,
                           GLdouble posX, GLdouble posY, GLdouble posZ);
    // Select partial border vertices by three clicks
    // @param mesh, the mesh that contains this border
    void selectPartialBorder(Mesh & mesh, GLint hits, GLuint *names,
                             GLdouble posX, GLdouble posY, GLdouble posZ);
};

#endif // SELECTION_H
