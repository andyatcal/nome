/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#include "offset.h"

Offset::Offset(Mesh & mesh, float val) {
    offsetVal = val;
    startMesh = mesh;
}

void Offset::makeSeperateOffset() {
    vector<Vertex*>::iterator vIt;
    vector<Face*>::iterator fIt;
    for(vIt = startMesh.vertList.begin(); vIt < startMesh.vertList.end(); vIt++) {
        calcVertexOffset(*vIt, false);
    }
    for(fIt = startMesh.faceList.begin(); fIt < startMesh.faceList.end(); fIt++) {
        vector<Vertex*> posOffsetVertices;
        vector<Vertex*> negOffsetVertices;
        Face * currFace = (*fIt);
        Edge * firstEdge = currFace -> oneEdge;
        if(firstEdge == NULL) {
            cout<<"ERROR: This face (with ID) does not have a sideEdge."<<endl;
            exit(1);
        }
        Edge * currEdge = firstEdge;
        Vertex * currVert;
        Edge * nextEdge;
        do {
            if(currFace == currEdge -> fa) {
                currVert = currEdge -> vb;
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    currVert = currEdge -> vb;
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    currVert = currEdge -> va;
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            if(!(currVert -> onMobius)) {
                posOffsetVertices.push_back(currVert -> posOffset);
                negOffsetVertices.push_back(currVert -> negOffset);
            } else {
                if(dot(currVert -> normal, currFace -> normal) >= 0) {
                    currVert -> negOffset -> normal = - currVert -> normal;
                    currVert -> posOffset -> normal = currVert -> normal;
                    posOffsetVertices.push_back(currVert -> posOffset);
                    negOffsetVertices.push_back(currVert -> negOffset);
                } else {
                    currVert -> negOffset -> normal = currVert -> normal;
                    currVert -> posOffset -> normal = - currVert -> normal;
                    posOffsetVertices.push_back(currVert -> negOffset);
                    negOffsetVertices.push_back(currVert -> posOffset);
                }
            }
            currEdge = nextEdge;
        } while(currEdge != firstEdge);
        posOffsetMesh.addPolygonFace(posOffsetVertices);
        negOffsetMesh.addPolygonFace(negOffsetVertices, true);

        firstEdge = currFace -> oneEdge;
        currEdge = firstEdge;
        do {
            if(currFace == currEdge -> fa) {
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            //cout<<"currEdge va: "<<nextEdge -> va -> ID<<endl;
            //cout<<"currEdge vb: "<<nextEdge -> vb -> ID<<endl;
            if(currEdge -> fb == NULL) {
                vector<Vertex*> sideOffsetVertices1;
                vector<Vertex*> sideOffsetVertices2;
                if(currEdge -> va -> onMobius) { // Probably need to fix this part because va and vb can be on Mobius at the same time.
                    if(dot(currEdge -> va -> normal, currEdge -> fa -> normal) >= 0) {
                        sideOffsetVertices1.push_back(currEdge -> va -> posOffset);
                        sideOffsetVertices1.push_back(currEdge -> va);
                        sideOffsetVertices1.push_back(currEdge -> vb);
                        sideOffsetVertices1.push_back(currEdge -> vb -> posOffset);
                        sideOffsetVertices2.push_back(currEdge -> va);
                        sideOffsetVertices2.push_back(currEdge -> va -> negOffset);
                        sideOffsetVertices2.push_back(currEdge -> vb -> negOffset);
                        sideOffsetVertices2.push_back(currEdge -> vb);
                    } else {
                        sideOffsetVertices1.push_back(currEdge -> va -> negOffset);
                        sideOffsetVertices1.push_back(currEdge -> va);
                        sideOffsetVertices1.push_back(currEdge -> vb);
                        sideOffsetVertices1.push_back(currEdge -> vb -> posOffset);
                        sideOffsetVertices2.push_back(currEdge -> va);
                        sideOffsetVertices2.push_back(currEdge -> va -> posOffset);
                        sideOffsetVertices2.push_back(currEdge -> vb -> negOffset);
                        sideOffsetVertices2.push_back(currEdge -> vb);
                    }
                } else if(currEdge -> vb -> onMobius) {
                    if(dot(currEdge -> vb -> normal, currEdge -> fa -> normal) >= 0) {
                        sideOffsetVertices1.push_back(currEdge -> va -> posOffset);
                        sideOffsetVertices1.push_back(currEdge -> va);
                        sideOffsetVertices1.push_back(currEdge -> vb);
                        sideOffsetVertices1.push_back(currEdge -> vb -> posOffset);
                        sideOffsetVertices2.push_back(currEdge -> va);
                        sideOffsetVertices2.push_back(currEdge -> va -> negOffset);
                        sideOffsetVertices2.push_back(currEdge -> vb -> negOffset);
                        sideOffsetVertices2.push_back(currEdge -> vb);
                    } else {
                        sideOffsetVertices1.push_back(currEdge -> va -> posOffset);
                        sideOffsetVertices1.push_back(currEdge -> va);
                        sideOffsetVertices1.push_back(currEdge -> vb);
                        sideOffsetVertices1.push_back(currEdge -> vb -> negOffset);
                        sideOffsetVertices2.push_back(currEdge -> va -> negOffset);
                        sideOffsetVertices2.push_back(currEdge -> vb -> posOffset);
                        sideOffsetVertices2.push_back(currEdge -> vb);
                        sideOffsetVertices2.push_back(currEdge -> va);
                    }
                } else {
                    sideOffsetVertices1.push_back(currEdge -> va -> posOffset);
                    sideOffsetVertices1.push_back(currEdge -> va);
                    sideOffsetVertices1.push_back(currEdge -> vb);
                    sideOffsetVertices1.push_back(currEdge -> vb -> posOffset);
                    sideOffsetVertices2.push_back(currEdge -> va);
                    sideOffsetVertices2.push_back(currEdge -> va -> negOffset);
                    sideOffsetVertices2.push_back(currEdge -> vb -> negOffset);
                    sideOffsetVertices2.push_back(currEdge -> vb);
                }
                sideOffsetMesh.addPolygonFace(sideOffsetVertices1);
                sideOffsetMesh.addPolygonFace(sideOffsetVertices2);
            }
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        posOffsetMesh.buildBoundary();
        negOffsetMesh.buildBoundary();
        posOffsetMesh.computeNormals();
        negOffsetMesh.computeNormals();
    }
}

void Offset::makeFullOffset() {
    vector<Vertex*>::iterator vIt;
    vector<Face*>::iterator fIt;
    for(vIt = startMesh.vertList.begin(); vIt < startMesh.vertList.end(); vIt++) {
        calcVertexOffset(*vIt, true);
    }
    for(fIt = startMesh.faceList.begin(); fIt < startMesh.faceList.end(); fIt++) {
        vector<Vertex*> posOffsetVertices;
        vector<Vertex*> negOffsetVertices;
        Face * currFace = (*fIt);
        Edge * firstEdge = currFace -> oneEdge;
        if(firstEdge == NULL) {
            cout<<"ERROR: This face (with ID) does not have a sideEdge."<<endl;
            exit(1);
        }
        Edge * currEdge = firstEdge;
        Vertex * currVert;
        Edge * nextEdge;
        do {
            if(currFace == currEdge -> fa) {
                currVert = currEdge -> vb;
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    currVert = currEdge -> vb;
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    currVert = currEdge -> va;
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            if(!(currVert -> onMobius)) {
                posOffsetVertices.push_back(currVert -> posOffset);
                negOffsetVertices.push_back(currVert -> negOffset);
            } else {
                if(dot(currVert -> normal, currFace -> normal) >= 0) {
                    posOffsetVertices.push_back(currVert -> posOffset);
                    negOffsetVertices.push_back(currVert -> negOffset);
                } else {
                    posOffsetVertices.push_back(currVert -> negOffset);
                    negOffsetVertices.push_back(currVert -> posOffset);
                }
            }
            currEdge = nextEdge;
        } while(currEdge != firstEdge);
        offsetMesh.addPolygonFace(posOffsetVertices);
        offsetMesh.addPolygonFace(negOffsetVertices, true);
        firstEdge = currFace -> oneEdge;
        currEdge = firstEdge;
        do {
            if(currFace == currEdge -> fa) {
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            if(currEdge -> fb == NULL) {
                vector<Vertex*> sideOffsetVertices;
                if(currEdge -> va -> onMobius) {
                    if(dot(currEdge -> va -> normal, currEdge -> fa -> normal) >= 0) {
                        sideOffsetVertices.push_back(currEdge -> va -> posOffset);
                        sideOffsetVertices.push_back(currEdge -> va -> negOffset);
                        sideOffsetVertices.push_back(currEdge -> vb -> negOffset);
                        sideOffsetVertices.push_back(currEdge -> vb -> posOffset);
                    } else {
                        sideOffsetVertices.push_back(currEdge -> va -> negOffset);
                        sideOffsetVertices.push_back(currEdge -> va -> posOffset);
                        sideOffsetVertices.push_back(currEdge -> vb -> negOffset);
                        sideOffsetVertices.push_back(currEdge -> vb -> posOffset);
                    }
                } else if(currEdge -> vb -> onMobius) {
                    if(dot(currEdge -> vb -> normal, currEdge -> fa -> normal) >= 0) {
                        sideOffsetVertices.push_back(currEdge -> va -> posOffset);
                        sideOffsetVertices.push_back(currEdge -> va -> negOffset);
                        sideOffsetVertices.push_back(currEdge -> vb -> negOffset);
                        sideOffsetVertices.push_back(currEdge -> vb -> posOffset);
                    } else {
                        sideOffsetVertices.push_back(currEdge -> va -> posOffset);
                        sideOffsetVertices.push_back(currEdge -> va -> negOffset);
                        sideOffsetVertices.push_back(currEdge -> vb -> posOffset);
                        sideOffsetVertices.push_back(currEdge -> vb -> negOffset);
                    }
                } else {
                    sideOffsetVertices.push_back(currEdge -> va -> posOffset);
                    sideOffsetVertices.push_back(currEdge -> va -> negOffset);
                    sideOffsetVertices.push_back(currEdge -> vb -> negOffset);
                    sideOffsetVertices.push_back(currEdge -> vb -> posOffset);
                }
                offsetMesh.addPolygonFace(sideOffsetVertices);
            }
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
    }
    offsetMesh.color = startMesh.color;
    offsetMesh.buildBoundary();
    offsetMesh.computeNormals();
}

void Offset::calcVertexOffset(Vertex * v, bool full) {
    Vertex * posOffset = new Vertex;
    Vertex * negOffset = new Vertex;
    posOffset -> position = v -> position + v -> normal * offsetVal;
    negOffset -> position = v -> position - v -> normal * offsetVal;
    posOffset -> ID = v -> ID + 2000000;
    negOffset -> ID = v -> ID + 4000000;
    posOffset -> normal = v -> normal;
    negOffset -> normal = - v -> normal;
    v -> posOffset = posOffset;
    v -> negOffset = negOffset;
    if(!full) {
        posOffsetMesh.addVertex(posOffset);
        negOffsetMesh.addVertex(negOffset);
        posOffsetMesh.addVertex(posOffset);
        negOffsetMesh.addVertex(negOffset);
        posOffsetMesh.addVertex(posOffset);
        negOffsetMesh.addVertex(negOffset);
        sideOffsetMesh.addVertex(v);
    } else {
        offsetMesh.addVertex(posOffset);
        offsetMesh.addVertex(negOffset);
    }
}
