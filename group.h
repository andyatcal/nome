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
    /* A list of pointers to Group under this group. */
    vector<Group*> subgroups;
    /* A list of pointers to Mesh under this group. */
    vector<Mesh*> myMeshes;
    /* A list of pointers to Polyline under this group. */
    vector<PolyLine*> myPolylines;
    /* The color of this group. */
    QColor color;
    /**
     * @brief addMesh: Add a mesh to this group.
     * @param mesh: The pointer to the added mesh.
     */
    void addMesh(Mesh* mesh);
    /**
     * @brief addGroup: Add a group to this group.
     * @param group: The pointer to the added group.
     */
    void addGroup(Group *group);
    /**
     * @brief addPolyline: Add a polyline to this group.
     * @param polyline: The pointer to the added polyline.
     */
    void addPolyline(PolyLine *polyline);
    /**
     * @brief drawMeshes: Draw meshes under this group.
     */
    void drawMeshes();
    /**
     * @brief drawPolylines: Draw polylines under this group.
     */
    void drawPolylines();
    /**
     * @brief flattenedMeshes. Flatten this group of its meshes.
     * @return all meshes under this group as a list.
     */
    vector<Mesh*> flattenedMeshes();
    /**
     * @brief flattenedPolylines: Flatten this group of its polylines.
     * @return all polylines under this group as a list.
     */
    vector<Polyline*> flattenedPolylines();
    /**
     * @brief assignMeshColor. Top down assigning colors to all its meshes.
     * If the mesh or subgroup has color, then skip them.
     */
    void assignMeshColor();
    /**
     * @brief assignPolylineColor. Top down assigning colors to all its polylines.
     * If the polyline or subgroup has color, then skip them.
     */
    void assignPolylineColor();
};

#endif // GROUP_H
