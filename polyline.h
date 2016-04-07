/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef __POLYLINE_H__
#define __POLYLINE_H__

#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"
#include "transformation.h"
using namespace std;
using namespace glm;

class Group;
//////////////////////////////////////////////////////////////////////
// Polyline Class -- A Polyline is a list of vertices
class PolyLine {
public:
    vector<Vertex*> vertices;
    bool isLoop;
    QColor color;
    PolyLine();
    /**
     * @brief drawLine: Draw this polyline in OpenGL
     */
    void drawLine(int start_index = 0);
    /**
     * @brief drawLineWithCubes: Draw this polygon with
     * a cube around every vertex.
     * @param start_index: the starting index of the name
     * buffer.
     */
    void drawLineWithCubes(int start_index = 0);
    // Return the skewness of points in this polyline.
    /**
     * @brief skewness: Find the skeness of all points.
     * @return the skewness of points in this polyline.
     */
    vector<float> skewness();
    /**
     * @brief clear: clear this Polyline.
     */
    void clear();
    /**
     * @brief isEmpty: Find if this line is empty.
     * @return indicator of this polyline is empty.
     */
    /**
     * @brief makeCopy: Make a copy of this PolyLine
     * @return the copied polyline
     */
    PolyLine makeCopy(string copy_polyline_name = "");
    /**
     * @brief transform: Transform this polyline.
     * @param t: The transformation for this polyline.
     */
    void transform(Transformation* t);
    /* Check if this polyline is empty.*/
    bool isEmpty();
    /* Set the color of this mesh. */
    void setColor(QColor color);
    /* Indicator of whether user sets the color of this mesh.*/
    bool user_set_color;
    /* The transformations to go up one level. */
    vector<Transformation> transformations_up;
    /* Set the transformation of this polyline. */
    void addTransformation(Transformation new_transform);
    /* Reset the transformations to this polyline of going up one level. */
    void setTransformation(vector<Transformation>);
    /* Add a vertex to the current polyline. */
    void addVertex(Vertex *v);
    /* parent group */
    Group *parent;
    /* The name of this polyline. */
    string name;
    /* Make a copy of this polyline in order for futrue transformation. */
    PolyLine makeCopyForTransform();
    /* The pointer to the polyline it copied from.*/
    PolyLine *before_transform_polyline;
    void updateCopyForTransform();
    /* A helper functoin for drawLineWithCubes.*/
    void drawCubeAroundVertex(Vertex *v, float size);
    /* specify the sizes of the cubes to draw in the scene. */
    vector<float> cubeSizes;
    /* A helper function for drawLineWithCubes.*/
    void updateCubeSizes();
};

#endif // __POLYLINE_H__
