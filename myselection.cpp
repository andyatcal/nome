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

void MySelection::selectFace(vector<Mesh*> &globalMeshList,
                             vector<int> &globalNameIndexList,
                             GLint hits,
                             GLuint *names)
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
        int currentID = names[minimumDepthIndex * 4 + 3];
        Face * workFace;
        vector<Mesh*>::iterator mIt;
        vector<int>::iterator nIt;
        for(mIt = globalMeshList.begin(), nIt = globalNameIndexList.begin();
            mIt != globalMeshList.end(); mIt ++, nIt++)
        {
            Mesh *currMesh = (*mIt);
            if(currentID < (currMesh -> faceList).size() + (*nIt))
            {
                workFace = currMesh -> faceList[currentID - (*nIt)];
                break;
            }
        }
        if(!workFace->selected)
        {
            workFace->selected = true;
        }
        else
        {
            workFace->selected = false;
        }
    }
}

void MySelection::selectFace(vector<Mesh*> &globalMeshList,
                             vector<PolyLine*> &globalPolylineList,
                             vector<int> &globalNameIndexList,
                             vector<int> &globalPolylineNameIndexList,
                             GLint hits, GLuint *names,
                             GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Face * selectedFace = NULL;
        Mesh * selectedMesh = NULL;
        int endOfMesh = INT_MAX;
        if(globalPolylineNameIndexList.size() > 0)
        {
            endOfMesh = globalPolylineNameIndexList[0];
        }
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace = NULL;
            Mesh * workMesh = NULL;
            vector<Mesh*>::iterator mIt;
            vector<int>::iterator nIt;
            /* Select a point on the polyline has a priority. */
            if(currentID < endOfMesh)
            {
                for(mIt = globalMeshList.begin(),
                    nIt = globalNameIndexList.begin();
                    mIt < globalMeshList.end(); mIt++, nIt++)
                {
                    Mesh *currMesh = (*mIt);
                    if(currentID < (currMesh -> faceList).size() + (*nIt))
                    {
                        workFace = currMesh -> faceList[currentID - (*nIt)];
                        workMesh = currMesh;
                        break;
                    }
                }
                if(workFace != NULL)
                {
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
                            selectedFace = workFace;
                            selectedMesh = workMesh;
                        }
                        currEdge = nextEdge;
                    } while (currEdge != firstEdge);
                }
            }
        }
        unordered_map<Mesh*, vector<Face*> >::iterator mfIt;
        if(selectedFace != NULL)
        {
            if(selectedFace -> selected)
            {
                selectedFace -> selected = false;
                mfIt = selectedFaces.find(selectedMesh);
                if(mfIt != selectedFaces.end())
                {
                    vector<Face*>::iterator fIt;
                    for(fIt = (mfIt -> second).begin();
                     fIt < (mfIt -> second).end(); fIt ++) {
                        if((*fIt) == selectedFace) {
                            break;
                        }
                    }
                    (mfIt -> second).erase(fIt);
                    cout<<"Unselected Face: "<<selectedFace -> name<<endl;
                }
                else
                {
                    cout<<"Error, there is a bug in the selectoin program!"<<endl;
                }
            }
            else
            {
                selectedFace -> selected = true;
                mfIt = selectedFaces.find(selectedMesh);
                if(mfIt == selectedFaces.end())
                {
                    vector<Face*> faces;
                    faces.push_back(selectedFace);
                    cout<<"Selected Face: "<<selectedFace -> name<<endl;
                    selectedFaces[selectedMesh] = faces;
                }
                else
                {
                    (mfIt -> second).push_back(selectedFace);
                    cout<<"Selected Face: "<<selectedFace -> name<<endl;
                }

            }
        }
        else
        {
            cout<<"You did not click on a face."<<endl;
        }
    }
}

void MySelection::selectVertex(vector<Mesh*> &globalMeshList,
                               vector<PolyLine*> &globalPolylineList,
                               vector<int> &globalNameIndexList,
                               vector<int> &globalPolylineNameIndexList,
                               GLint hits, GLuint *names,
                               GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex = NULL;
        int endOfMesh = INT_MAX;
        if(globalPolylineNameIndexList.size() > 0)
        {
            endOfMesh = globalPolylineNameIndexList[0];
        }
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace = NULL;
            Vertex *workVertex = NULL;
            vector<Mesh*>::iterator mIt;
            vector<PolyLine*>::iterator pIt;
            vector<int>::iterator nIt;
            /* Select a point on the polyline has a priority. */
            if(currentID >= endOfMesh)
            {
                for(pIt = globalPolylineList.begin(),
                    nIt = globalPolylineNameIndexList.begin();
                    pIt < globalPolylineList.end(); pIt++, nIt++)
                {
                    PolyLine *currLine = (*pIt);
                    if(currentID < (currLine -> vertices).size() + (*nIt))
                    {
                        workVertex = currLine -> vertices[currentID - (*nIt)];
                        float new_distance = distance(workVertex -> position,
                                                      hit_position);
                        if(new_distance < min_distance) {
                            min_distance = new_distance;
                            selectedVertex = workVertex;
                        }
                        break;
                    }
                }
                if(selectedVertex != NULL)
                {
                    goto getSelectedVertex;
                }
            }
            else
            {
                for(mIt = globalMeshList.begin(),
                    nIt = globalNameIndexList.begin();
                    mIt < globalMeshList.end(); mIt++, nIt++)
                {
                    Mesh *currMesh = (*mIt);
                    if(currentID < (currMesh -> faceList).size() + (*nIt))
                    {
                        workFace = currMesh -> faceList[currentID - (*nIt)];
                        break;
                    }
                }
                if(workFace != NULL)
                {
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
            }
        }
        getSelectedVertex:
        if(selectedVertex -> source_vertex != NULL)
        {
            selectedVertex = selectedVertex -> source_vertex;
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
            cout<<"Unselected Vertex: "<<selectedVertex -> name<<endl;
            cout<<"You have "<<selectedVertices.size()
            <<" vertices selected."<<endl;
        } else {
            selectedVertex -> selected = true;
            selectedVertices.push_back(selectedVertex);
            cout<<"Selected Vertex: "<<selectedVertex -> name<<endl;
            cout<<"You have "<<selectedVertices.size()
            <<" vertices selected."<<endl;
        }
    }
}

void MySelection::selectVertex(vector<Mesh*> &globalMeshList,
                               vector<int> &globalNameIndexList,
                               GLint hits, GLuint *names,
                               GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace;
            vector<Mesh*>::iterator mIt;
            vector<int>::iterator nIt;
            for(mIt = globalMeshList.begin(), nIt = globalNameIndexList.begin();
                mIt != globalMeshList.end(); mIt ++, nIt++)
            {
                Mesh *currMesh = (*mIt);
                if(currentID < (currMesh -> faceList).size() + (*nIt))
                {
                    workFace = currMesh -> faceList[currentID - (*nIt)];
                    break;
                }
            }
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
            cout<<"Unselected Vertex: "<<selectedVertex -> name<<endl;
            cout<<"You have "<<selectedVertices.size()
            <<" vertices selected."<<endl;
        } else {
            selectedVertex -> selected = true;
            selectedVertices.push_back(selectedVertex);
            cout<<"Selected Vertex: "<<selectedVertex -> name<<endl;
            cout<<"You have "<<selectedVertices.size()
            <<" vertices selected."<<endl;
        }
    }
}

void MySelection::selectWholeBorder(vector<Mesh*> &globalMeshList,
                                    vector<int> &globalNameIndexList,
                                    GLint hits, GLuint *names,
                                    GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace;
            vector<Mesh*>::iterator mIt;
            vector<int>::iterator nIt;
            for(mIt = globalMeshList.begin(), nIt = globalNameIndexList.begin();
                mIt != globalMeshList.end(); mIt ++, nIt++)
            {
                Mesh *currMesh = (*mIt);
                if(currentID < (currMesh -> faceList).size() + (*nIt))
                {
                    workFace = currMesh -> faceList[currentID - (*nIt)];
                    break;
                }
            }
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

void MySelection::selectPartialBorder(vector<Mesh*> &globalMeshList,
                                      vector<int> &globalNameIndexList,
                                      GLint hits, GLuint *names,
                                      GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace;
            vector<Mesh*>::iterator mIt;
            vector<int>::iterator nIt;
            for(mIt = globalMeshList.begin(), nIt = globalNameIndexList.begin();
                mIt != globalMeshList.end(); mIt ++, nIt++)
            {
                Mesh *currMesh = (*mIt);
                if(currentID < (currMesh -> faceList).size() + (*nIt))
                {
                    workFace = currMesh -> faceList[currentID - (*nIt)];
                    break;
                }
            }
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

void MySelection::selectWholeBorder(vector<Mesh*> &globalMeshList,
                                    vector<PolyLine*> &globalPolylineList,
                                    vector<int> &globalNameIndexList,
                                    vector<int> &globalPolylineNameIndexList,
                                    GLint hits, GLuint *names,
                                    GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex = NULL;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace = NULL;
            vector<Mesh*>::iterator mIt;
            vector<int>::iterator nIt;
            for(mIt = globalMeshList.begin(), nIt = globalNameIndexList.begin();
                mIt != globalMeshList.end(); mIt ++, nIt++)
            {
                Mesh *currMesh = (*mIt);
                if(currentID < (currMesh -> faceList).size() + (*nIt))
                {
                    workFace = currMesh -> faceList[currentID - (*nIt)];
                    break;
                }
            }
            if(workFace != NULL)
            {
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
            else
            {
                cout<<"Please click on a face."<<endl;
            }
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

void MySelection::selectPartialBorder(vector<Mesh*> &globalMeshList,
                                      vector<PolyLine*> &globalPolylineList,
                                      vector<int> &globalNameIndexList,
                                      vector<int> &globalPolylineNameIndexList,
                                      GLint hits, GLuint *names,
                                      GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace;
            vector<Mesh*>::iterator mIt;
            vector<int>::iterator nIt;
            for(mIt = globalMeshList.begin(), nIt = globalNameIndexList.begin();
                mIt != globalMeshList.end(); mIt ++, nIt++)
            {
                Mesh *currMesh = (*mIt);
                if(currentID < (currMesh -> faceList).size() + (*nIt))
                {
                    workFace = currMesh -> faceList[currentID - (*nIt)];
                    break;
                }
            }
            if(workFace != NULL)
            {
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
            else
            {
                cout<<"Please click on a face."<<endl;
            }
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
    for(vIt = selectedVertices.begin(); vIt < selectedVertices.end(); vIt++)
    {
        (*vIt) -> selected = false;
    }
    unordered_map<Mesh*, vector<Face*> >::iterator mfIt;
    for(mfIt = selectedFaces.begin(); mfIt != selectedFaces.end(); mfIt++) {
        vector<Face*> faces = mfIt -> second;
        for(Face * face : faces) {
            face -> selected = false;
        }
    }
    selectedVertices.clear();
    selectedFaces.clear();
    firstBorderSelectionPoint = NULL;
    secondBorderSelectionPoint = NULL;
    allBorderPoints.clear();
    vertToSelect.clear();
}

void MySelection::addSelectedToMesh(Mesh &mesh)
{
    vector<Vertex*> vertices;
    vertices.clear();
    vector<Vertex*>::iterator vIt;
    Vertex * foundVertex;
    for(vIt = selectedVertices.begin();
     vIt < selectedVertices.end(); vIt++)
    {
        foundVertex = NULL;
        for(Vertex * v : mesh.vertList)
        {
            if((v -> source_vertex) == (*vIt))
            {
                foundVertex = v;
                break;
            }
        }
        if(foundVertex != NULL)
        {
            vertices.push_back(foundVertex);
        }
        else
        {   Vertex * newVertex = new Vertex;
            newVertex -> source_vertex = *vIt;
            newVertex -> position = (*vIt) -> position;
            newVertex -> ID = mesh.vertList.size();
            mesh.addVertex(newVertex);
            vertices.push_back(newVertex);
        }
    }
    mesh.addPolygonFace(vertices);
    mesh.buildBoundary();
    mesh.computeNormals();
    clearSelection();
}

void MySelection::deleteSelectedFaces(vector<string> & deletedFaces)
{
    unordered_map<Mesh*, vector<Face*> >::iterator mfIt;
    for(mfIt = selectedFaces.begin(); mfIt != selectedFaces.end(); mfIt++)
    {
        Mesh * mesh = mfIt -> first;
        vector<Face*> faces = mfIt -> second;
        for(Face * face : faces)
        {
            if(mesh -> type != 99)
            {
                deletedFaces.push_back(face -> name);
            }
            face -> selected = false;
            mesh -> deleteFace(face);
        }
        mesh -> buildBoundary();
        mesh -> computeNormals();
        if(mesh -> isConsolidateMesh)
        {
            mesh -> updateVertListAfterDeletion();
        }
    }
    clearSelection();
}

PolyLine MySelection::addSelectedToPolyline(bool isLoop)
{
    PolyLine newBorder;
    if(selectedVertices.size() < 2) {
        cout<<"A Line have size greater than 1."<<endl;
        clearSelection();
        return newBorder;
    }
    vector<Vertex*>::iterator vIt;
    for(vIt = selectedVertices.begin();
      vIt < selectedVertices.end(); vIt ++) {
        newBorder.vertices.push_back(*vIt);
    }
    newBorder.isLoop = isLoop;
    clearSelection();
    return newBorder;
}
