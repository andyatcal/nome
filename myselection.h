/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef __MYSELECTION_H__
#define __MYSELECTION_H__

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>
#include <unordered_map>
#include "polyline.h"
#include "mesh.h"

using namespace glm;
using namespace std;

//////////////////////////////////////////////////////////////////////
// Selection Class -- Handles Mouse Selection.

class MySelection
{
private:
    /* Faces selected. Can be used to delete Face or change colors.*/
    unordered_map<Mesh*, vector<Face*> > selectedFaces;
    /* For partial border selection/*/
    Vertex * firstBorderSelectionPoint = NULL;
    Vertex * secondBorderSelectionPoint = NULL;
    vector<Vertex*> allBorderPoints;
    vector<Vertex*> firstHalfBorder;
    vector<Vertex*> vertToSelect;
public:
    /* Vertices selected. Can be used to create Face or Polyline.*/
    vector<Vertex*> selectedVertices;
    /**
     * @brief list_hits: A debug function. Shows the current hit
     * objects names.
     * @param hits: The returned hits from GL_SELECT mode name buffer.
     * Indicating objects hit by the clicking ray.
     * @param names: The user defined name buffer.
     */
    void list_hits(GLint hits, GLuint *names);
    /**
     * @brief: Select the hitted face from mesh.
     * @param mesh: the mesh that contains this face.
     * @param hits: The returned hits from GL_SELECT mode name buffer.
     * Indicating objects hit by the clicking ray.
     * @param names: The user defined name buffer.
     */
    void selectFace(vector<Mesh*> &globalMeshList, vector<int> &globalNameIndexList,
                    GLint hits, GLuint *names);
    void selectFace(vector<Mesh*> &globalMeshList,
                    vector<PolyLine*> &globalPolylineList,
                    vector<int> &globalNameIndexList,
                    vector<int> &globalPolylineNameIndexList,
                    GLint hits, GLuint *names,
                    GLdouble posX, GLdouble posY, GLdouble posZ);
    /**
     * @brief selectVertex: Select the nearest vertex.
     * @param mesh: the mesh that contains this face.
     * @param hits: The returned hits from GL_SELECT mode name buffer.
     * Indicating objects hit by the clicking ray.
     * @param names: The user defined name buffer.
     * @param posX: X position from the actual hit point on polygon.
     * @param posY: Y position from the actual hit point on polygon.
     * @param posZ: Z position from the actual hit point on polygon.
     */
    void selectVertex(vector<Mesh*> &globalMeshList,
                      vector<int> &globalNameIndexList,
                      GLint hits, GLuint *names,
                      GLdouble posX, GLdouble posY, GLdouble posZ);
    void selectVertex(vector<Mesh*> &globalMeshList,
                      vector<PolyLine*> &globalPolylineList,
                      vector<int> &globalNameIndexList,
                      vector<int> &globalPolyllineNameIndexList,
                      GLint hits, GLuint *names,
                      GLdouble posX, GLdouble posY, GLdouble posZ);
    // Select all border vertices

    /**
     * @brief selectWholeBorder: Select all vertices from a border.
     * The border is the cloest one to the clicking ray hit.
     * @param mesh: the mesh that contains this face.
     * @param hits: The returned hits from GL_SELECT mode name buffer.
     * Indicating objects hit by the clicking ray.
     * @param names: The user defined name buffer.
     * @param posX: X position from the actual hit point on polygon.
     * @param posY: Y position from the actual hit point on polygon.
     * @param posZ: Z position from the actual hit point on polygon.
     */
    void selectWholeBorder(vector<Mesh*> &globalMeshList,
                           vector<int> &globalNameIndexList,
                           GLint hits, GLuint *names,
                           GLdouble posX, GLdouble posY, GLdouble posZ);
    void selectWholeBorder(vector<Mesh*> &globalMeshList,
                      vector<PolyLine*> &globalPolylineList,
                      vector<int> &globalNameIndexList,
                      vector<int> &globalPolyllineNameIndexList,
                      GLint hits, GLuint *names,
                      GLdouble posX, GLdouble posY, GLdouble posZ);
    /**
     * @brief selectPartialBorder: Select all vertices from a border.
     * The border is the cloest one to the clicking ray hit.
     * @param mesh: the mesh that contains this face.
     * @param hits: The returned hits from GL_SELECT mode name buffer.
     * Indicating objects hit by the clicking ray.
     * @param names: The user defined name buffer.
     * @param posX: X position from the actual hit point on polygon.
     * @param posY: Y position from the actual hit point on polygon.
     * @param posZ: Z position from the actual hit point on polygon.
     */
    void selectPartialBorder(vector<Mesh*> &globalMeshList,
                             vector<int> &globalNameIndexList,
                             GLint hits, GLuint *names,
                             GLdouble posX, GLdouble posY, GLdouble posZ);
    void selectPartialBorder(vector<Mesh*> &globalMeshList,
                      vector<PolyLine*> &globalPolylineList,
                      vector<int> &globalNameIndexList,
                      vector<int> &globalPolyllineNameIndexList,
                      GLint hits, GLuint *names,
                      GLdouble posX, GLdouble posY, GLdouble posZ);
    /**
     * @brief: clear the current selection
     */
    void clearSelection();
    /**
     * @brief addSelectedToMesh: Add the selected vertices as a polygon to
     * a mesh.
     * @param mesh. The destiny mesh.
     */
    void addSelectedToMesh(Mesh &mesh);
    /**
     * @brief addSelectedToPolyline: Add the selected vertices to a new polyline.
     * @param isLoop: indicate if this polyline is a loop.
     * @return pointer to the new polyline
     */
    PolyLine addSelectedToPolyline(bool isLoop = false);
    /* Delete the faces that we have selected. */
    void deleteSelectedFaces(vector<string> & deletedFaces);
};

#endif // SELECTION_H
