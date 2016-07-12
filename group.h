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
     * @brief flattenedMeshes. A flattened view of meshes in this group.
     * @return all meshes under this group as a list.
     */
    vector<Mesh*> flattenedMeshes();
    /**
     * @brief flattenedPolylines: A flattened view of polylines in this group.
     * @return all polylines under this group as a list.
     */
    vector<PolyLine*> flattenedPolylines();
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
    /* The transformations to go up one level. */
    vector<Transformation> transformations_up;
    /* Set the transformation of this group. */
    void addTransformation(Transformation new_transform);
    /* Set the transformation of this group. */
    void setTransformation(vector<Transformation> new_transforms);
    /* make a copy of this group.*/
    Group makeCopy(string copy_group_name = "");
    /* make a copy of this group for future transformation. */
    Group makeCopyForTransform();
    /* Update me based on what I copied from .*/
    void updateCopyForTransform();
    /* Clear this group.*/
    void clear();
    /* Clear and delete everything from this group.*/
    void clearAndDelete();
    /* name of this group. */
    string name;
    /* set name of this group. */
    void setName(string);
    /* The parent of this group. */
    Group* parent;
    /**
     * Build maps from parameters to the leaves of this group.
     */
    void mapFromParameters();
    /* A pointer to the original group before transformation. */
    Group * before_transform_group;
    /* Update the names for all elements in the group. Used for the final scene.*/
    void updateGroupElementName();
    /* Find a vertex given its hierarchical name.*/
    Vertex* findVertexInThisGroup(string name);
    /* Find a face given its hierachical name and delete this face. */
    bool deleteFaceInThisGroup(string name);
    /* Indicator that this group is in editing mode.
     * So we can't change the hyper parameters of this mesh,
     * e.g. the parameter n of the mesh.
     */
    bool in_editing_mode;
};

#endif // GROUP_H
