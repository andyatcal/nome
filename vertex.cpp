//
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
//
//

#include "vertex.h"

Vertex::Vertex(){
    position = vec3(0, 0, 0);
    normal = vec3(0, 0, 0);
    oneEdge = NULL;
    ID = 0;
    selected = false;
}

Vertex::Vertex(float x, float y, float z, unsigned long ID) {
    position = vec3(x, y, z);
    normal = vec3(0, 0, 0);
    oneEdge = NULL;
    ID = ID;
    selected = false;
}
