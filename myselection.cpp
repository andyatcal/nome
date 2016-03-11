/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#include "myselection.h"

void MySelection::list_hits(GLint hits, GLuint *names)
{
    int i;

      /*
         For each hit in the buffer are allocated 4 bytes:
         1. Number of hits selected (always one,
         beacuse when we draw each object
         we use glLoadName, so we replace the
         prevous name in the stack)
         2. Min Z
         3. Max Z
         4. Name of the hit (glLoadName)
         */

    printf("%d hits:\n", hits);

    for (i = 0; i < hits; i++)
        printf( "Number: %d\n"
                "Min Z: %d\n"
                "Max Z: %d\n"
                "Name on stack: %d\n",
                (GLubyte)names[i * 4],
                (GLubyte)names[i * 4 + 1],
                (GLubyte)names[i * 4 + 2],
                (GLubyte)names[i * 4 + 3]
            );
        printf("\n");
}

void MySelection::selectFace(Mesh &mesh, GLint hits, GLuint *names)
{
    if(hits > 0) {
        int minimumDepth = INT_MAX;
        int minimumDepthIndex = INT_MAX;
        for (int i = 0; i < hits; i++) {
            int currentDepth = (GLubyte)names[i * 4 + 1];
            if(currentDepth < minimumDepth) {
                minimumDepth = currentDepth;
                minimumDepthIndex = i;
            }
        }
        int selectedID = names[minimumDepthIndex * 4 + 3];
        Face * workFace = mesh.faceList[selectedID];
        if(!workFace->selected) {
            workFace->selected = true;
        } else {
            workFace->selected = false;
        }
    }
}

void MySelection::selectVertex(Mesh & mesh, GLint hits, GLuint *names,
                  GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            if(currentID > mesh.faceList.size()) {
                continue;
            }
            Face * workFace = mesh.faceList[currentID];
            Edge * firstEdge = workFace -> oneEdge;
            Edge * currEdge = firstEdge;
            Edge * nextEdge;
            Vertex * tempv;
            do {
                if(workFace == currEdge -> fa) {
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
                float new_distance = distance(tempv -> position, hit_position);
                if(new_distance < min_distance) {
                    min_distance = new_distance;
                    selectedVertex = tempv;
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
        }
        if(selectedVertex -> selected) {
            selectedVertex -> selected = false;
            vector<Vertex*>::iterator vIt;
            for(vIt = selectedVertices.begin();
             vIt < selectedVertices.end(); vIt ++) {
                if((*vIt) == selectedVertex) {
                    break;
                }
            }
            selectedVertices.erase(vIt);
            cout<<"Unselected Vertex: v"<<selectedVertex -> ID<<endl;
            cout<<"You have "<<selectedVertices.size()
            <<" vertices selected."<<endl;
        } else {
            selectedVertex -> selected = true;
            selectedVertices.push_back(selectedVertex);
            cout<<"Selected Vertex: v"<<selectedVertex -> ID<<endl;
            cout<<"You have "<<selectedVertices.size()
            <<" vertices selected."<<endl;
        }
    }
}

void MySelection::selectWholeBorder(Mesh &mesh, GLint hits, GLuint *names,
                       GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            if(currentID > mesh.faceList.size()) {
                continue;
            }
            Face * workFace = mesh.faceList[currentID];
            Edge * firstEdge = workFace -> oneEdge;
            Edge * currEdge = firstEdge;
            Edge * nextEdge;
            Vertex * tempv;
            do {
                if(workFace == currEdge -> fa) {
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
                float new_distance = distance(tempv -> position, hit_position);
                if(new_distance < min_distance) {
                    min_distance = new_distance;
                    selectedVertex = tempv;
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
        }
        // Test is this point is on border. If yes, find the startingEdge.
        if(selectedVertex != NULL) {
            Edge * firstEdge = selectedVertex -> oneEdge;
            Edge * currEdge = firstEdge;
            Face * currFace = currEdge -> fb;
            Edge * nextEdge;
            do {
                if(currFace == NULL) {
                    break;
                } else {
                    nextEdge = currEdge -> nextEdge(selectedVertex, currFace);
                    currFace = nextEdge -> theOtherFace(currFace);
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
            if(currFace != NULL) {
                cout<<"Your selected vertex is not on the border."
                <<" Current Selection Cancelled.\n";
                return;
            }
            if(selectedVertex -> selected) {
                cout<<"Unselecting all points on this border.\n";
                selectedVertices.clear();
                Vertex * nextVert = selectedVertex;
                Edge * nextBorderEdge = currEdge;
                do {
                    nextVert -> selected = false;
                    //cout<<nextVert -> ID;
                    if(nextVert == nextBorderEdge -> va) {
                        nextBorderEdge = nextBorderEdge -> nextVaFb;
                    } else {
                        nextBorderEdge = nextBorderEdge -> nextVbFb;
                    }
                    if(nextBorderEdge -> va == nextVert) {
                        nextVert = nextBorderEdge -> vb;
                    } else {
                        nextVert = nextBorderEdge -> va;
                    }
                } while (nextVert != selectedVertex);
            } else {
                Vertex * nextVert = selectedVertex;
                Edge * nextBorderEdge = currEdge;
                do {
                    selectedVertices.push_back(nextVert);
                    nextVert -> selected = true;
                    //cout<<nextVert -> ID;
                    if(nextVert == nextBorderEdge -> va) {
                        nextBorderEdge = nextBorderEdge -> nextVaFb;
                    } else {
                        nextBorderEdge = nextBorderEdge -> nextVbFb;
                    }
                    if(nextBorderEdge -> va == nextVert) {
                        nextVert = nextBorderEdge -> vb;
                    } else {
                        nextVert = nextBorderEdge -> va;
                    }
                } while (nextVert != selectedVertex);
                cout<<"You have selected "<<selectedVertices.size()
                <<" points on this border.\n";
            }
        }
    }
}

void MySelection::selectPartialBorder(Mesh & mesh, GLint hits, GLuint *names,
                         GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace = mesh.faceList[currentID];
            Edge * firstEdge = workFace -> oneEdge;
            Edge * currEdge = firstEdge;
            Edge * nextEdge;
            Vertex * tempv;
            do {
                if(workFace == currEdge -> fa) {
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
                float new_distance = distance(tempv -> position, hit_position);
                if(new_distance < min_distance) {
                    min_distance = new_distance;
                    selectedVertex = tempv;
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
        }
        // Test is this point is on border. If yes, find the startingEdge.
        if(selectedVertex != NULL) {
            Edge * firstEdge = selectedVertex -> oneEdge;
            Edge * currEdge = firstEdge;
            Face * currFace = currEdge -> fb;
            Edge * nextEdge;
            do {
                if(currFace == NULL) {
                    break;
                } else {
                    nextEdge = currEdge -> nextEdge(selectedVertex, currFace);
                    currFace = nextEdge -> theOtherFace(currFace);
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
            if(currFace != NULL) {
                cout<<"Your selected vertex is not on the border."
                <<" Current Selection Cancelled.\n";
                return;
            }
            if (!firstBorderSelectionPoint) {
                firstBorderSelectionPoint = selectedVertex;
                cout<<"Selecting the first Point on border."<<endl;
                selectedVertex -> selected = true;
                selectedVertices.push_back(selectedVertex);
                Vertex * nextVert = selectedVertex;
                Edge * nextBorderEdge = currEdge;
                do {
                    allBorderPoints.push_back(nextVert);
                    //cout<<nextVert -> ID<<endl;;
                    if(nextVert == nextBorderEdge -> va) {
                        nextBorderEdge = nextBorderEdge -> nextVaFb;
                    } else {
                        nextBorderEdge = nextBorderEdge -> nextVbFb;
                    }
                    if(nextBorderEdge -> va == nextVert) {
                        nextVert = nextBorderEdge -> vb;
                    } else {
                        nextVert = nextBorderEdge -> va;
                    }
                } while (nextVert != selectedVertex);
            } else if (!secondBorderSelectionPoint) {
                // We need to test if this second point in the border
                // of the first selecte point.
                vector<Vertex*>::iterator vIt;
                for(vIt = allBorderPoints.begin();
                 vIt < allBorderPoints.end(); vIt++) {
                    if((*vIt) == selectedVertex) {
                        if((*vIt) == firstBorderSelectionPoint) {
                            cout<<"Unselecting the first point on border."<<endl;
                            firstBorderSelectionPoint -> selected = false;
                            firstBorderSelectionPoint = NULL;
                            allBorderPoints.clear();
                            selectedVertices.clear();
                            //(*vIt) -> selected = false;
                        }
                        break;
                    }
                }
                if(!allBorderPoints.empty()) {
                    if(vIt == allBorderPoints.end()) {
                        cout<<"Your choice of the two points are not on the"
                        <<" same border. Selection ABORT."<<endl;
                        firstBorderSelectionPoint -> selected = false;
                        firstBorderSelectionPoint = NULL;
                        allBorderPoints.clear();
                        selectedVertices.clear();
                    } else {
                        cout<<"Selecting the second point on border."<<endl;
                        secondBorderSelectionPoint = selectedVertex;
                        selectedVertex -> selected = true;
                        selectedVertices.push_back(selectedVertex);
                    }
                }
            } else if(!vertToSelect.empty()) {
                cout<<"Unselecting all points on partial border."<<endl;
                vector<Vertex*>::iterator vIt;
                for(vIt = vertToSelect.begin(); vIt < vertToSelect.end(); vIt++) {
                    (*vIt) -> selected = false;
                }
                firstBorderSelectionPoint -> selected = false;
                secondBorderSelectionPoint -> selected = false;
                firstBorderSelectionPoint = NULL;
                secondBorderSelectionPoint = NULL;
                allBorderPoints.clear();
                vertToSelect.clear();
                selectedVertices.clear();
            } else {
                vector<Vertex*>::iterator vIt;
                bool hasSeenSecondPoint = false;
                for(vIt = allBorderPoints.begin(); vIt < allBorderPoints.end(); vIt++) {
                    if((*vIt) == selectedVertex) {
                        if((*vIt) == firstBorderSelectionPoint
                         || (*vIt) == secondBorderSelectionPoint) {
                            cout<<"Unselecting points on border."<<endl;
                            firstBorderSelectionPoint -> selected = false;
                            secondBorderSelectionPoint -> selected = false;
                            firstBorderSelectionPoint = NULL;
                            secondBorderSelectionPoint = NULL;
                            allBorderPoints.clear();
                            selectedVertices.clear();
                        }
                        break;
                    } else if((*vIt) == secondBorderSelectionPoint) {
                        hasSeenSecondPoint = true;
                    }
                }
                if(!allBorderPoints.empty()) {
                    if(vIt == allBorderPoints.end()) {
                        cout<<"Your choice of the third points is not on"
                         <<"the same border. Selection ABORT."<<endl;
                        firstBorderSelectionPoint -> selected = false;
                        secondBorderSelectionPoint -> selected = false;
                        firstBorderSelectionPoint = NULL;
                        secondBorderSelectionPoint = NULL;
                        allBorderPoints.clear();
                        selectedVertices.clear();
                    } else {
                        cout<<"Selecting this half border on this side."<<endl;
                        vector<Vertex*>::iterator vSelIt;
                        if(!hasSeenSecondPoint) {
                            for(vSelIt = allBorderPoints.begin();
                             vSelIt < allBorderPoints.end(); vSelIt++) {
                                vertToSelect.push_back(*vSelIt);
                                if((*vSelIt) == secondBorderSelectionPoint) {
                                    break;
                                }
                            }
                        } else {
                            vertToSelect.push_back(firstBorderSelectionPoint);
                            for(vSelIt = allBorderPoints.end() - 1;
                             vSelIt >= allBorderPoints.begin(); vSelIt--) {
                                vertToSelect.push_back(*vSelIt);
                                if((*vSelIt) == secondBorderSelectionPoint) {
                                    break;
                                }
                            }
                        }
                        selectedVertices.clear();
                        for(vSelIt = vertToSelect.begin();
                         vSelIt < vertToSelect.end(); vSelIt++) {
                            (*vSelIt) -> selected = true;
                            selectedVertices.push_back(*vSelIt);
                        }
                        cout<<"You have selected "<<selectedVertices.size()
                        <<" points on this border.\n";
                    }
                }
            }
        }
    }
}

void MySelection::clearSelection() {
    vector<Vertex*>::iterator vIt;
    for(vIt = selectedVertices.begin(); vIt < selectedVertices.end(); vIt++) {
        (*vIt) -> selected = false;
    }
    selectedVertices.clear();
    firstBorderSelectionPoint = NULL;
    secondBorderSelectionPoint = NULL;
    allBorderPoints.clear();
    vertToSelect.clear();
}

void MySelection::addSelectedToMesh(Mesh &mesh)
{
    vector<Vertex*> vertices;
    vector<Vertex*>::iterator vIt;
    for(vIt = selectedVertices.begin();
     vIt < selectedVertices.end(); vIt++)
    {
        mesh.addVertex(*vIt);
        vertices.push_back(*vIt);
    }
    mesh.addPolygonFace(vertices);
    mesh.buildBoundary();
    mesh.computeNormals();
    clearSelection();
}

PolyLine *MySelection::addSelectedToPolyline(bool isLoop)
{
    if(selectedVertices.size() < 2) {
        cout<<"A Line have size greater than 1."<<endl;
        clearSelection();
        return NULL;
    }
    vector<Vertex*>::iterator vIt;
    PolyLine *newBorder;
    for(vIt = selectedVertices.begin();
      vIt < selectedVertices.end(); vIt ++) {
        newBorder->vertices.push_back(*vIt);
    }
    newBorder->isLoop = isLoop;
    clearSelection();
    return newBorder;
}
