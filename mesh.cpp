/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#include "mesh.h"

Mesh::Mesh()
{

}

void Mesh::addVertex(Vertex * v)
{
    vertList.push_back(v);
}

Edge * Mesh::findEdge(Vertex * v1, Vertex * v2)
{
    unordered_map<Vertex*, vector<Edge*> >::iterator vIt;
    vector<Edge*> currEdges;
    vector<Edge*>::iterator eIt;
    vIt = edgeTable.find(v2);
    if(vIt != edgeTable.end())
    {
        for(eIt = (vIt -> second).begin(); eIt < (vIt -> second).end(); eIt ++)
        {
            if((*eIt) -> vb == v1)
            {
                //cout<<"Find Edge from vertex "<<v2 -> ID<<" to vertex "<<v1 -> ID<<"."<<endl;
                return (*eIt);
            }
        }
    }
    vIt = edgeTable.find(v1);
    if(vIt != edgeTable.end()) {
        for(eIt = (vIt -> second).begin(); eIt < (vIt -> second).end(); eIt ++) {
            if((*eIt) -> vb == v2) {
                //cout<<"Find M Edge from vertex "<<v1 -> ID<<" to vertex "<<v2 -> ID<<"."<<endl;
                (*eIt) -> mobius = true;
                (*eIt) -> va -> onMobius = true;
                (*eIt) -> vb -> onMobius = true;
                return (*eIt);
            }
        }
    }
    return NULL;
}

Edge * Mesh::createEdge(Vertex * v1, Vertex * v2) {
    Edge * edge = findEdge(v1, v2);
    if(edge == NULL) {
        //cout<<"Creating new Edge from vertex "<<v1 -> ID<<" to vertex "<<v2 -> ID<<"."<<endl;
        edge = new Edge(v1, v2);
        if(v1 -> oneEdge == NULL) {
            v1 -> oneEdge = edge;
        }
        if(v2 -> oneEdge == NULL) {
            v2 -> oneEdge = edge;
        }
        unordered_map<Vertex*, vector<Edge*> >::iterator vIt;
        vIt = edgeTable.find(v1);
        if(vIt != edgeTable.end()) {
            (vIt -> second).push_back(edge);
        } else {
            vector<Edge*> currEdges;
            currEdges.push_back(edge);
            edgeTable[v1] = currEdges;
        }
    }
    //cout<<"The va of edge is "<<edge -> va -> ID<<" . The vb is "<< edge -> vb -> ID<<" ."<<endl;
    return edge;
}

void Mesh::addTriFace(Vertex * v1, Vertex * v2, Vertex * v3) {
    Face * newFace = new Face;
    Edge * e12 = createEdge(v1, v2);
    Edge * e23 = createEdge(v2, v3);
    Edge * e31 = createEdge(v3, v1);
    if(e12 -> fa == NULL) {
        e12 -> fa = newFace;
    } else if(e12 -> fb == NULL) {
        e12 -> fb = newFace;
    } else {
        cout<<"ERROR: Try to create a Non-Manifold at edge with vertex1 : "
        <<v1 -> ID<<" and vertex2 :"<<v2 -> ID<<endl;
        exit(0);
    }
    if(e23 -> fa == NULL) {
        e23 -> fa = newFace;
    } else if(e23 -> fb == NULL) {
        e23 -> fb = newFace;
    } else {
        cout<<"ERROR: Try to create a Non-Manifold at edge with vertex1 : "
        <<v2 -> ID<<" and vertex2 :"<<v3 -> ID<<endl;
        exit(0);
    }
    if(e31 -> fa == NULL) {
        e31 -> fa = newFace;
    } else if(e31 -> fb == NULL) {
        e31 -> fb = newFace;
    } else {
        cout<<"ERROR: Try to create a Non-Manifold at edge with vertex1 : "
        <<v3 -> ID<<" and vertex2 :"<<v1 -> ID<<endl;
        exit(0);
    }
    newFace -> oneEdge = e12;
    //cout<<"Testing: "<<newFace -> oneEdge -> va -> ID<<endl;
    e12 -> setNextEdge(v1, newFace, e31);
    e12 -> setNextEdge(v2, newFace, e23);
    e23 -> setNextEdge(v2, newFace, e12);
    e23 -> setNextEdge(v3, newFace, e31);
    e31 -> setNextEdge(v1, newFace, e12);
    e31 -> setNextEdge(v3, newFace, e23);
    newFace -> id = faceList.size();
    faceList.push_back(newFace);
}

void Mesh::addQuadFace(Vertex * v1, Vertex * v2, Vertex * v3, Vertex * v4) {
    Face * newFace = new Face;
    Edge * e12 = createEdge(v1, v2);
    Edge * e23 = createEdge(v2, v3);
    Edge * e34 = createEdge(v3, v4);
    Edge * e41 = createEdge(v4, v1);
    if(e12 -> fa == NULL) {
        e12 -> fa = newFace;
    } else if(e12 -> fb == NULL) {
        e12 -> fb = newFace;
    } else {
        cout<<"ERROR: Try to create a Non-Manifold at edge with vertex1 : "
        <<v1 -> ID<<" and vertex2 :"<<v2 -> ID<<endl;
        exit(0);
    }
    if(e23 -> fa == NULL) {
        e23 -> fa = newFace;
    } else if(e23 -> fb == NULL) {
        e23 -> fb = newFace;
    } else {
        cout<<"ERROR: Try to create a Non-Manifold at edge with vertex1 : "
        <<v2 -> ID<<" and vertex2 :"<<v3 -> ID<<endl;
        exit(0);
    }
    if(e34 -> fa == NULL) {
        e34 -> fa = newFace;
    } else if(e34 -> fb == NULL) {
        e34 -> fb = newFace;
    } else {
        cout<<"ERROR: Try to create a Non-Manifold at edge with vertex1 : "
        <<v3 -> ID<<" and vertex2 :"<<v4 -> ID<<endl;
        exit(0);
    }
    if(e41 -> fa == NULL) {
        e41 -> fa = newFace;
    } else if(e41 -> fb == NULL) {
        e41 -> fb = newFace;
    } else {
        cout<<"ERROR: Try to create a Non-Manifold at edge with vertex1 : "
        <<v4 -> ID<<" and vertex2 :"<<v1 -> ID<<endl;
        exit(0);
    }
    newFace -> oneEdge = e12;
    e12 -> setNextEdge(v1, newFace, e41);
    e12 -> setNextEdge(v2, newFace, e23);
    e23 -> setNextEdge(v2, newFace, e12);
    e23 -> setNextEdge(v3, newFace, e34);
    e34 -> setNextEdge(v3, newFace, e23);
    e34 -> setNextEdge(v4, newFace, e41);
    e41 -> setNextEdge(v4, newFace, e34);
    e41 -> setNextEdge(v1, newFace, e12);
    newFace -> id = faceList.size();
    faceList.push_back(newFace);
}

void Mesh::addPolygonFace(vector<Vertex*> vertices, bool reverseOrder) {
    if(vertices.size() < 3) {
        cout<<"A face have at least 3 vertices"<<endl;
        return;
    }
    Face * newFace = new Face;
    vector<Vertex*>::iterator vIt;
    vector<Edge*> edgesInFace;
    vector<Edge*>::iterator eIt;
    Edge * currEdge;
    if(!reverseOrder) {
        for(vIt = vertices.begin(); vIt < vertices.end(); vIt ++) {
            if(vIt != vertices.end() - 1) {
                currEdge = createEdge(*vIt, *(vIt + 1));
                edgesInFace.push_back(currEdge);
            } else {
                currEdge = createEdge(*vIt, *(vertices.begin()));
                edgesInFace.push_back(currEdge);
            }
            if(currEdge -> fa == NULL) {
                currEdge -> fa = newFace;
            } else if(currEdge -> fb == NULL) {
                currEdge -> fb = newFace;
            } else {
                cout<<"ERROR: Try to create a Non-Manifold at edge with vertex1 : "
                <<currEdge -> va -> ID<<" and vertex2 :"<<currEdge -> vb -> ID<<endl;
                exit(0);
            }
        }
    } else {
        for(vIt = vertices.end() - 1; vIt >= vertices.begin(); vIt --) {
            if(vIt != vertices.begin()) {
                currEdge = createEdge(*vIt, *(vIt - 1));
                edgesInFace.push_back(currEdge);
            } else {
                currEdge = createEdge(*vIt, *(vertices.end() - 1));
                edgesInFace.push_back(currEdge);
            }
            if(currEdge -> fa == NULL) {
                currEdge -> fa = newFace;
            } else if(currEdge -> fb == NULL) {
                currEdge -> fb = newFace;
            } else {
                cout<<"ERROR: Try to create a Non-Manifold at edge with vertex1 : "
                <<currEdge -> va -> ID<<" and vertex2 :"<<currEdge -> vb -> ID<<endl;
                exit(0);
            }
        }
    }
    newFace -> oneEdge = currEdge;
    for(eIt = edgesInFace.begin(); eIt < edgesInFace.end(); eIt ++) {
        Edge * currEdge = (*eIt);
        if(eIt == edgesInFace.begin()) {
            if(newFace == currEdge -> fa) {
                currEdge -> nextVbFa = *(eIt + 1);
                currEdge -> nextVaFa = *(edgesInFace.end() - 1);
            } else {
                if(currEdge -> mobius) {
                    currEdge -> nextVbFb = *(eIt + 1);
                    currEdge -> nextVaFb = *(edgesInFace.end() - 1);
                } else {
                    currEdge -> nextVaFb = *(eIt + 1);
                    currEdge -> nextVbFb = *(edgesInFace.end() - 1);
                }
            }
        } else if(eIt == (edgesInFace.end() - 1)) {
            if(newFace == currEdge -> fa) {
                currEdge -> nextVbFa = *(edgesInFace.begin());
                currEdge -> nextVaFa = *(eIt - 1);
            } else {
                if(currEdge -> mobius) {
                    currEdge -> nextVbFb = *(edgesInFace.begin());
                    currEdge -> nextVaFb = *(eIt - 1);
                } else {
                    currEdge -> nextVaFb = *(edgesInFace.begin());
                    currEdge -> nextVbFb = *(eIt - 1);
                }
            }
        } else {
            if(newFace == currEdge -> fa) {
                currEdge -> nextVbFa = *(eIt + 1);
                currEdge -> nextVaFa = *(eIt - 1);
            } else {
                if(currEdge -> mobius) {
                    currEdge -> nextVbFb = *(eIt + 1);
                    currEdge -> nextVaFb = *(eIt - 1);
                } else {
                    currEdge -> nextVaFb = *(eIt + 1);
                    currEdge -> nextVbFb = *(eIt - 1);
                }
            }
        }
    }
    newFace -> id = faceList.size();
    faceList.push_back(newFace);
}

// Build the next pointers for boundary edges in the mesh.
// @param mesh: refer to the mesh to build connection in.
// This one takes O(E) time.
void Mesh::buildBoundary() {
    unordered_map<Vertex*, vector<Edge*> >::iterator evIt;
    vector<Edge*> edgesAtThisPoint;
    vector<Edge*>::iterator eIt;
    for(evIt = edgeTable.begin(); evIt != edgeTable.end(); evIt++) {
        edgesAtThisPoint = evIt -> second;
        if(!edgesAtThisPoint.empty()) {
            for(eIt = edgesAtThisPoint.begin(); eIt < edgesAtThisPoint.end(); eIt++) {
                Edge * currEdge = (*eIt);
                if((currEdge -> nextEdge(currEdge -> va, currEdge -> fb)) == NULL) {
                    Edge * firstBoundaryEdge = currEdge;
                    Vertex * currVert = currEdge -> va;
                    Edge * nextBoundaryEdge;
                    //cout<<"first: "<<currEdge -> va -> ID<<" "<<currEdge -> vb -> ID<<endl;
                    do {
                        currEdge -> isSharp = true;
                        //cout<<"Now building boundary at vertex: "<<endl;
                        //cout<<currVert -> ID<<endl;
                        Face * currFace = currEdge -> fa;
                        nextBoundaryEdge = currEdge -> nextEdge(currVert, currFace);
                        while(nextBoundaryEdge -> fb != NULL) {
                            currFace = nextBoundaryEdge -> theOtherFace(currFace);
                            nextBoundaryEdge = nextBoundaryEdge -> nextEdge(currVert, currFace);
                        }
                        currEdge -> setNextEdge(currVert, NULL, nextBoundaryEdge);
                        nextBoundaryEdge -> setNextEdge(currVert, NULL, currEdge);
                        currEdge = nextBoundaryEdge;
                        currVert = currEdge -> theOtherVertex(currVert);
                    } while (currEdge != firstBoundaryEdge);
                }
            }
        }
    }
}

// @param p1, p2, p3 are positions of three vertices,
// with edge p1 -> p2 and edge p2 -> p3.
vec3 getNormal3Vertex(vec3 p1, vec3 p2, vec3 p3){
    return cross(p2 - p1, p3 - p2);
}

// Get the surface normal.
// @param currFace: pointer of the face.
void getFaceNormal(Face * currFace){
    //cout<<"New Face!"<<endl;
    Edge * firstEdge = currFace -> oneEdge;
    Edge * currEdge;
    Edge * nextEdge;
    currEdge = firstEdge;
    vec3 avgNorm = vec3(0, 0, 0);
    vec3 p1;
    vec3 p2;
    vec3 p3;
    do {
        //cout<<"New Edge!"<<endl;
        //cout<<"ID: "<<currEdge -> va -> ID<<endl;
        //cout<<"ID: "<<currEdge -> vb -> ID<<endl;
        if(currFace == currEdge -> fa) {
            nextEdge = currEdge -> nextVbFa;
            p1 = currEdge -> va -> position;
            p2 = currEdge -> vb -> position;
            p3 = nextEdge -> theOtherVertex(currEdge -> vb) -> position;
        } else if(currFace == currEdge -> fb) {
            if(currEdge -> mobius) {
                nextEdge = currEdge -> nextVbFb;
                p1 = currEdge -> va -> position;
                p2 = currEdge -> vb -> position;
                p3 = nextEdge -> theOtherVertex(currEdge -> vb) -> position;
            } else {
                nextEdge = currEdge -> nextVaFb;
                p1 = currEdge -> vb -> position;
                p2 = currEdge -> va -> position;
                p3 = nextEdge -> theOtherVertex(currEdge -> va) -> position;
            }
        }
        avgNorm += getNormal3Vertex(p1, p2, p3);
        currEdge = nextEdge;
    } while (currEdge != firstEdge);
    vec3 result = normalize(avgNorm);
    //cout<<"The new Face normal is: "<<result[0]<<" "<<result[1]<<" "<<result[2]<<endl;
    currFace -> normal = normalize(avgNorm);
}

// Get the vertex normal
// @param currVert: pointer of the edge, which the vertex on the end of.
void getVertexNormal(Vertex * currVert){
    Edge * firstEdge = currVert -> oneEdge;
    if(firstEdge == NULL) {
        cout<<"Lonely vertex without any adjacent edges"<<endl;
        return;
    }
    Edge * currEdge = firstEdge;
    Face * currFace = currEdge -> fa;
    vec3 avgNorm(0, 0, 0);
    int mobiusCounter = 0;
    do {
        if(mobiusCounter % 2 == 0) {
            avgNorm += currFace -> normal;
        } else {
            avgNorm -= currFace -> normal;
        }
        if(currEdge -> mobius) {
            mobiusCounter += 1;
        }
        currFace = currEdge -> theOtherFace(currFace);
        if(currFace == NULL) { //If the face is NULL, need to skip this face
            Edge * nextEdge = currEdge -> nextEdge(currVert, currFace);
            if(nextEdge -> va == currEdge -> va || nextEdge -> vb == currEdge -> vb) {
                mobiusCounter += 1;
            }
            currEdge = nextEdge;
            currFace = currEdge -> theOtherFace(currFace);
        }
        currEdge = currEdge -> nextEdge(currVert, currFace);
    } while ( currEdge != firstEdge);
    //if(currVert -> onMobius) {
    //cout<<"The value of avgNorm is :"<<avgNorm[0]<<" "<<avgNorm[1]<<" "<<avgNorm[2]<<endl;
    //cout<<"The position of this vertex is :"<<currVert -> position[0]<<" "<<currVert -> position[1]<<" "<<currVert -> position[2]<<endl;
    //}//cout<<"ID: "<<currVert -> ID <<" has "<<mobiusCounter<<" mConter"<<endl;
    currVert -> normal = normalize(avgNorm);
}

// Iterate over every vertex in the mesh and compute its normal
void Mesh::computeNormals(){
    vector<Vertex*>::iterator vIt;
    vector<Face*>::iterator fIt;
    //cout<<"faceTable size: "<<faceList.size()<<endl;
    for(fIt = faceList.begin(); fIt < faceList.end(); fIt++) {
        getFaceNormal(*fIt);
    }

    //cout<<"vertTable size: "<<vertList.size()<<endl;
    for(vIt = vertList.begin(); vIt != vertList.end(); vIt++) {
        //cout<<"Now calculating vertex with ID: "<< vIt -> first <<endl;
        getVertexNormal(*vIt);
    }

}

void Mesh::drawMesh() {
    Face * tempFace;
    vector<Face*>::iterator fIt;
    for(fIt = faceList.begin(); fIt < faceList.end(); fIt++) {
        tempFace = (*fIt);
        vec3 fNormal = tempFace -> normal;
        Vertex * tempv;
        Edge * firstEdge = (*fIt) -> oneEdge;
        //cout<<"New Face: "<<endl;
        glLoadName(tempFace -> id);
        if(tempFace -> selected) {
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, PURPLE);
        }
        glBegin(GL_POLYGON);
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        //tempv = currEdge -> va;
        //cout<<"Hmm?"<<endl;
        //cout<<"Hello! I am on the "<<fIt - faceList.begin()<<" face."<<endl;
        do {
            if(tempFace == currEdge -> fa) {
                tempv = currEdge -> vb;
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    tempv = currEdge -> vb;
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    tempv = currEdge -> va;
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            float normx;
            float normy;
            float normz;
            if(tempv -> onMobius) {
                vec3 vNormal = tempv -> normal;
                if(dot(vNormal, fNormal) >= 0) {
                    normx = tempv -> normal[0];
                    normy = tempv -> normal[1];
                    normz = tempv -> normal[2];
                } else {
                    normx = - tempv -> normal[0];
                    normy = - tempv -> normal[1];
                    normz = - tempv -> normal[2];
                }
            } else {
                normx = tempv -> normal[0];
                normy = tempv -> normal[1];
                normz = tempv -> normal[2];
            }
            //cout<<"normx: "<<normx<<" normy: "<<normy<<" normz: "<<normz<<endl;
            float x = tempv -> position[0];
            float y = tempv -> position[1];
            float z = tempv -> position[2];
            glNormal3f(normx, normy, normz);
            //cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<endl;
            glVertex3f(x, y, z);
            currEdge = nextEdge;
            //cout<<"Current Vertex ID: "<<tempv -> ID<<endl;
        } while(currEdge != firstEdge);
        glEnd();
        //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, CYAN);
    }
}

vector<Edge*> Mesh::boundaryEdgeList() {
    vector<Edge*> boundaryEdgeList;
    unordered_map<Vertex*, vector<Edge*> >::iterator vIt;
    //cout<<"Edge table size: "<<edgeTable.size()<<endl;
    for(vIt = edgeTable.begin(); vIt != edgeTable.end(); vIt++) {
        //cout<<vIt -> first -> ID<<endl;
        vector<Edge*> edges = vIt -> second;
        vector<Edge*>::iterator eIt;
        vector<Edge*> newEdges;
        for(eIt = edges.begin(); eIt < edges.end(); eIt ++) {
            if((*eIt) -> fb == NULL) {
                boundaryEdgeList.push_back(*eIt);
            }
        }
    }
    //cout<<"size: " << boundaryEdgeList.size()<<endl;
    return boundaryEdgeList;
}

void Mesh::drawVertices() {
    vector<Vertex*>::iterator vIt;
    glPointSize(10);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    int counter = 0;
    for(vIt = vertList.begin(); vIt < vertList.end(); vIt++) {
        if((*vIt) -> selected) {
            glBegin(GL_POINTS);
            vec3 position = (*vIt) -> position;
            glNormal3f(position[0] * 100, position[1] * 100, position[2] * 100);
            glVertex3f(position[0], position[1], position[2]);
            glEnd();
        }
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, YELLOW);
}

bool Mesh::empty() {
    return vertList.size() == 0 && faceList.size() == 0;
}

void Mesh::clear() {
    vertList.clear();
    faceList.clear();
    edgeTable.clear();
}
