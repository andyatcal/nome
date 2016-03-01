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
        Edge * firstEdge = workFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        Vertex * tempv;
        Vertex * selectedVertex;
        float minDistance = 50000.0; // A very large value ...
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
            float newDistance = distance(tempv -> position, vec3(posX, posY, posZ));
            if(newDistance < minDistance) {
                minDistance = newDistance;
                selectedVertex = tempv;
            }
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        if(selectedVertex -> selected) {
            selectedVertex -> selected = false;
        } else {
            selectedVertex -> selected = true;
        }
    }
}

void MySelection::selectWholeBorder(Mesh & mesh, GLint hits, GLuint *names,
                       GLdouble posX, GLdouble posY, GLdouble posZ)
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
        Edge * firstEdge = workFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        Vertex * selectedVertex = NULL;
        Edge * startingEdge;
        Vertex * tempva;
        Vertex * tempvb;
        float minDistance = 50000.0; // A very large value ...
        do {
            if(workFace == currEdge -> fa) {
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            if(currEdge -> fb == NULL) {
                tempva = currEdge -> va;
                tempvb = currEdge -> vb;
                float newDistanceA = distance(tempva -> position, vec3(posX, posY, posZ));
                float newDistanceB = distance(tempvb -> position, vec3(posX, posY, posZ));
                if(minDistance > newDistanceA) {
                    minDistance = newDistanceA;
                    selectedVertex = tempva;
                    startingEdge = currEdge;
                }
                if(minDistance > newDistanceB) {
                    minDistance = newDistanceB;
                    selectedVertex = tempvb;
                    startingEdge = currEdge;
                }
            }
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        if(selectedVertex != NULL) {
            if(selectedVertex -> selected) {
                Vertex * nextVert = selectedVertex;
                Edge * nextBorderEdge = startingEdge;
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
                Edge * nextBorderEdge = startingEdge;
                do {
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
            }
        }
    }
}

void MySelection::selectPartialBorder(Mesh & mesh, GLint hits, GLuint *names,
                         GLdouble posX, GLdouble posY, GLdouble posZ)
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
        Edge * firstEdge = workFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        Vertex * selectedVertex = NULL;
        Edge * startingEdge;
        Vertex * tempva;
        Vertex * tempvb;
        float minDistance = 50000.0; // A very large value ...
        do {
            if(workFace == currEdge -> fa) {
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            if(currEdge -> fb == NULL) {
                tempva = currEdge -> va;
                tempvb = currEdge -> vb;
                float newDistanceA = distance(tempva -> position, vec3(posX, posY, posZ));
                float newDistanceB = distance(tempvb -> position, vec3(posX, posY, posZ));
                if(minDistance > newDistanceA) {
                    minDistance = newDistanceA;
                    selectedVertex = tempva;
                    startingEdge = currEdge;
                }
                if(minDistance > newDistanceB) {
                    minDistance = newDistanceB;
                    selectedVertex = tempvb;
                    startingEdge = currEdge;
                }
            }
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        if(selectedVertex) {
            if (!firstBorderSelectionPoint) {
                firstBorderSelectionPoint = selectedVertex;
                cout<<"Selecting the first Point on border."<<endl;
                selectedVertex -> selected = true;
                Vertex * nextVert = selectedVertex;
                Edge * nextBorderEdge = startingEdge;
                do {
                    firstHalfBorder.push_back(nextVert);
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
                //cout<<firstHalfBorder.size()<<endl;
            } else if (!secondBorderSelectionPoint) {
                //We need to test if this second point in the border of the first selecte point.
                vector<Vertex*>::iterator vIt;
                for(vIt = firstHalfBorder.begin(); vIt < firstHalfBorder.end(); vIt++) {
                    if((*vIt) == selectedVertex) {
                        if((*vIt) == firstBorderSelectionPoint) {
                            cout<<"Unselecting the first point on border."<<endl;
                            firstBorderSelectionPoint -> selected = false;
                            firstBorderSelectionPoint = NULL;
                            firstHalfBorder.clear();
                            //(*vIt) -> selected = false;
                        }
                        break;
                    }
                }
                if(!firstHalfBorder.empty()) {
                    if(vIt == firstHalfBorder.end()) {
                        cout<<"Your choice of the two points is not on the same border. Selection ABORT."<<endl;
                        firstBorderSelectionPoint -> selected = false;
                        firstBorderSelectionPoint = NULL;
                        firstHalfBorder.clear();
                    } else {
                        cout<<"Selecting the second Point on border."<<endl;
                        secondBorderSelectionPoint = selectedVertex;
                        selectedVertex -> selected = true;
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
                firstHalfBorder.clear();
                vertToSelect.clear();
            } else {
                vector<Vertex*>::iterator vIt;
                bool hasSeenSecondPoint = false;
                for(vIt = firstHalfBorder.begin(); vIt < firstHalfBorder.end(); vIt++) {
                    if((*vIt) == selectedVertex) {
                        if((*vIt) == firstBorderSelectionPoint || (*vIt) == secondBorderSelectionPoint) {
                            cout<<"Unselecting points on border."<<endl;
                            firstBorderSelectionPoint -> selected = false;
                            secondBorderSelectionPoint -> selected = false;
                            firstBorderSelectionPoint = NULL;
                            secondBorderSelectionPoint = NULL;
                            firstHalfBorder.clear();
                        }
                        break;
                    } else if((*vIt) == secondBorderSelectionPoint) {
                        hasSeenSecondPoint = true;
                    }
                }
                if(!firstHalfBorder.empty()) {
                    if(vIt == firstHalfBorder.end()) {
                        cout<<"Your choice of the third points is not on the same border. Selection ABORT."<<endl;
                        firstBorderSelectionPoint -> selected = false;
                        secondBorderSelectionPoint -> selected = false;
                        firstBorderSelectionPoint = NULL;
                        secondBorderSelectionPoint = NULL;
                        firstHalfBorder.clear();
                    } else {
                        cout<<"Selecting the half border on this side."<<endl;
                        vector<Vertex*>::iterator vSelIt;
                        if(!hasSeenSecondPoint) {
                            for(vSelIt = firstHalfBorder.begin(); vSelIt < firstHalfBorder.end(); vSelIt++) {
                                vertToSelect.push_back(*vSelIt);
                                if((*vSelIt) == secondBorderSelectionPoint) {
                                    break;
                                }
                            }
                        } else {
                            vertToSelect.push_back(firstBorderSelectionPoint);
                            for(vSelIt = firstHalfBorder.end() - 1; vSelIt >= firstHalfBorder.begin(); vSelIt--) {
                                vertToSelect.push_back(*vSelIt);
                                if((*vSelIt) == secondBorderSelectionPoint) {
                                    break;
                                }
                            }
                        }
                        for(vSelIt = vertToSelect.begin(); vSelIt < vertToSelect.end(); vSelIt++) {
                            (*vSelIt) -> selected = true;
                        }
                    }
                }
            }
        }
    }
}
