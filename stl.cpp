//
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
//
//

#include "stl.h"

vec3 getTriFaceNormal(Vertex * va, Vertex * vb, Vertex * vc){
    vec3 pa = va -> position;
    vec3 pb = vb -> position;
    vec3 pc = vc -> position;
    vec3 vNormalb = getNormal3Vertex(pa, pb, pc);
    vec3 vNormalc = getNormal3Vertex(pb, pc, pa);
    vec3 vNormala = getNormal3Vertex(pc, pa, pb);
    return normalize(vNormala + vNormalb + vNormalc);
}

void STL::STLOutput(vector<Mesh> &meshes, string outputSTL){
    vector<Mesh>::iterator mIt;
    vector<Face*>::iterator faceIt;
    ofstream file(outputSTL);
    if (!file.is_open()) {
        cout <<"COULD NOT OPEN THE FILE.\n";
    } else {
        file << "solid\n";
        for(mIt = meshes.begin(); mIt < meshes.end(); mIt++) {
            Mesh & currMesh = (*mIt);
            vector<Face*>::iterator fIt;
            for(fIt = currMesh.faceList.begin(); fIt < currMesh.faceList.end(); fIt++) {
                Face * currFace = (*fIt);
                Edge * firstEdge = currFace -> oneEdge;
                Edge * currEdge;
                if(firstEdge == NULL) {
                    cout<<"ERROR: This face does not have a sideEdge."<<endl;
                    exit(0);
                }
                Vertex * v0, * v1, * v2;
                if(currFace == firstEdge -> fa) {
                    v0 = firstEdge -> va;
                    currEdge = firstEdge -> nextVbFa;
                } else {
                    if(firstEdge -> mobius) {
                        v0 = firstEdge -> va;
                        currEdge = firstEdge -> nextVbFb;
                    } else {
                        v0 = firstEdge -> vb;
                        currEdge = firstEdge -> nextVaFb;
                    }
                }
                vec3 p0 = v0 -> position;
                if(currEdge == NULL) {
                    cout<<"ERROR: This face contains only one edge and can not be drawn."<<endl;
                }
                do {
                    Edge * nextEdge;
                    if(currFace == currEdge -> fa) {
                        v1 = currEdge -> va;
                        v2 = currEdge -> vb;
                        nextEdge = currEdge -> nextVbFa;
                    } else {
                        if(currEdge -> mobius) {
                            v1 = currEdge -> va;
                            v2 = currEdge -> vb;
                            nextEdge = currEdge -> nextVbFb;
                        } else {
                            v1 = currEdge -> vb;
                            v2 = currEdge -> va;
                            nextEdge = currEdge -> nextVaFb;
                        }
                    }
                    if(v2 != v0) {
                        vec3 faceNormal = getTriFaceNormal(v0, v1, v2);
                        file << "  facet normal "<<faceNormal[0]<<" "<<faceNormal[1]<<" "<<faceNormal[2]<<"\n";
                        file << "    outer loop\n";
                        vec3 p1 = v1 -> position;
                        vec3 p2 = v2 -> position;
                        file << "      vertex " << p0[0] << " "<< p0[1] << " " << p0[2]<<"\n";
                        file << "      vertex " << p1[0] << " "<< p1[1] << " " << p1[2]<<"\n";
                        file << "      vertex " << p2[0] << " "<< p2[1] << " " << p2[2]<<"\n";
                        file << "    endloop\n";
                        file << "  endfacet\n";
                    }
                    currEdge = nextEdge;
                } while (currEdge != firstEdge);
            }
        }
        file << "endsolid\n";
    }
}

void STL::STLOutput(Mesh &currMesh, string outputSTL){
    vector<Face*>::iterator faceIt;
    ofstream file(outputSTL);
    if (!file.is_open()) {
        cout <<"Error: COULD NOT OPEN THE FILE.\n";
    } else {
        file << "solid\n";
        vector<Face*>::iterator fIt;
        for(fIt = currMesh.faceList.begin(); fIt < currMesh.faceList.end(); fIt++) {
            Face * currFace = (*fIt);
            Edge * firstEdge = currFace -> oneEdge;
            Edge * currEdge;
            if(firstEdge == NULL) {
                cout<<"ERROR: This face does not have a sideEdge."<<endl;
                exit(0);
            }
            Vertex * v0, * v1, * v2;
            if(currFace == firstEdge -> fa) {
                v0 = firstEdge -> va;
                currEdge = firstEdge -> nextVbFa;
            } else {
                if(firstEdge -> mobius) {
                    v0 = firstEdge -> va;
                    currEdge = firstEdge -> nextVbFb;
                } else {
                    v0 = firstEdge -> vb;
                    currEdge = firstEdge -> nextVaFb;
                }
            }
            vec3 p0 = v0 -> position;
            if(currEdge == NULL) {
                cout<<"ERROR: This face contains only one edge and can not be drawn."<<endl;
            }
            do {
                Edge * nextEdge;
                if(currFace == currEdge -> fa) {
                    v1 = currEdge -> va;
                    v2 = currEdge -> vb;
                    nextEdge = currEdge -> nextVbFa;
                } else {
                    if(currEdge -> mobius) {
                        v1 = currEdge -> va;
                        v2 = currEdge -> vb;
                        nextEdge = currEdge -> nextVbFb;
                    } else {
                        v1 = currEdge -> vb;
                        v2 = currEdge -> va;
                        nextEdge = currEdge -> nextVaFb;
                    }
                }
                if(v2 != v0) {
                    vec3 faceNormal = getTriFaceNormal(v0, v1, v2);
                    file << "  facet normal "<<faceNormal[0]<<" "<<faceNormal[1]<<" "<<faceNormal[2]<<"\n";
                    file << "    outer loop\n";
                    vec3 p1 = v1 -> position;
                    vec3 p2 = v2 -> position;
                    file << "      vertex " << p0[0] << " "<< p0[1] << " " << p0[2]<<"\n";
                    file << "      vertex " << p1[0] << " "<< p1[1] << " " << p1[2]<<"\n";
                    file << "      vertex " << p2[0] << " "<< p2[1] << " " << p2[2]<<"\n";
                    file << "    endloop\n";
                    file << "  endfacet\n";
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
        }
        file << "endsolid\n";
    }
}
