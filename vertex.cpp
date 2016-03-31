/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#include "vertex.h"

Vertex::Vertex(){
    position = vec3(0, 0, 0);
    normal = vec3(0, 0, 0);
    oneEdge = NULL;
    ID = 0;
    selected = false;
    before_transform_vertex = NULL;
}

Vertex::Vertex(float x, float y, float z, unsigned long ID) {
    position = vec3(x, y, z);
    normal = vec3(0, 0, 0);
    oneEdge = NULL;
    ID = ID;
    selected = false;
    before_transform_vertex = NULL;
}
