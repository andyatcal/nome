/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef GROUP_H
#define GROUP_H

#include "mesh.h"
#include "polyline.h"
#include "transformation.h"
#include "merge.h"
#include <vector>
#include <unordered_map>
#include <QColor>

class Group
{
public:
    Group();
    /* A list of subgroup under this group. */
    vector<Group> subgroups;
    /* A list of mesh under this group. */
    vector<Mesh> myMeshes;
    /* A list of polyline under this group. */
    vector<PolyLine> myPolylines;
    /* The color of this group. */
    QColor color;
    /* Indicator of whether user sets the color of this group.*/
    bool user_set_color;
    /**
     * @brief addMesh: Add a mesh to this group.
     * @param mesh: The pointer to the added mesh.
     */
    void addMesh(Mesh &mesh);
    /**
     * @brief addGroup: Add a group to this group.
     * @param group: The pointer to the added group.
     */
    void addGroup(Group &group);
    /**
     * @brief addPolyline: Add a polyline to this group.
     * @param polyline: The pointer to the added polyline.
     */
    void addPolyline(PolyLine &polyline);
    /**
     * @brief flattenedMeshes. A flattened view of this group.
     * @return all meshes under this group as a list.
     */
    vector<Mesh*> flattenedMeshes();
    /**
     * @brief flattenedPolylines: Flatten this group of its polylines.
     * @return all polylines under this group as a list.
     */
    vector<PolyLine> flattenedPolylines();
    /**
     * @brief assignMeshColor. Top to bottom assigning my color
     * to all sub meshes and polylines. If any mesh or subgroup
     * has user set color, then skip them.
     */
    void assignColor();
    /**
     * @brief merge: Merge this group into a single mesh.
     * @return The merged mesh.
     */
    Mesh merge();
    /**
     * @brief setColor: Set the color of this group.
     */
    void setColor(QColor color);
    /* The transformation to go up one level. */
    mat4 transformUp;
    /* Set the transformation of this group. */
    void setTransformation(mat4 new_transformation);
    /* make a copy of this group.*/
    Group makeCopy();
    /* Clear this group.*/
    void clear();
};

#endif // GROUP_H
