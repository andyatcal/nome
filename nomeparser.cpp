#include "nomeparser.h"

NomeParser::NomeParser()
{

}

bool testComments(string token)
{
    if(token[0] == '#') {
        return true;
    }
    return false;
}

/**
 * @brief warning: Helper function to output warnings.
 * @param type:
 *          0: Parameter reading error.
 *          1: Stoi exception. Line skipped.
 * @param lineNumber: The current line number.
 * @return
 */
string warning(int type, int lineNumber)
{
    switch(type)
    {
    case 0:
        return "Warning: parameter at line "
                + to_string(lineNumber) + " is not set because of insufficient parameters.";
    case 1:
        return "Warning: line " + to_string(lineNumber)
                + " contains string that can't be parsed, skipping this line.";
    case 2:
        return "Warning: parameter at line "
                + to_string(lineNumber) + " has duplicated names of existing parameters.";
    case 3:
        return "Warning: mesh at line "
                + to_string(lineNumber) + " has duplicated names of existing meshes.";
    case 4:
        return "Warning: group at line "
                + to_string(lineNumber) + " has duplicated names of existing groups.";
    case 5:
        return "Warninng: instance at line "
                + to_string(lineNumber) + " has not been created yet.";
    case 6:
        return "Warning: new instance at line"
                + to_string(lineNumber) + " does not have a name.";
    case 7:
        return "Warning: parameter name at line"
                + to_string(lineNumber) + " does not match any defined parameter.";
    case 8:
        return "Warning: parameter at line"
                + to_string(lineNumber) + " does not have a retored value.";
    case 9:
        return "Warning: vertex at line"
                + to_string(lineNumber) + " can't be restored.";
    case 10:
        return "Warning: point at line"
                + to_string(lineNumber) + " doesn't have a name. It can't be initiated.";
    case 11:
        return "Warning: point at line"
                + to_string(lineNumber) + " has already been created (duplicate names)." +
                                          " It can't be initiated.";
    case 12:
        return "Warning: polyline at line"
                + to_string(lineNumber) + " doesn't have a name. It can't be initiated.";
    case 13:
        return "Warning: polyline at line"
                + to_string(lineNumber) + " has already been created (duplicate names)." +
                                          " It can't be initiated.";
    case 14:
        return "Warning: vertex at line"
                + to_string(lineNumber) + " can't be added to polyline. It has not been created.";
    case 15:
        return "Warning: Color at line"
                + to_string(lineNumber) + " can't be assigned. It has not been created.";
    case 16:
        return "Warning: Color at line"
                + to_string(lineNumber) + " has already been created (duplicate names).";
    case 17:
        return "Warning: Color at line"
                + to_string(lineNumber) + " does not have enough input.";
    case 18:
        return "Warning: Instance at line"
                + to_string(lineNumber) + " does not complete color definition. " +
                "It can't be instantiated.";
    case 19:
        return "Warning: face at line"
                + to_string(lineNumber) + " doesn't have a name. It can't be initiated.";
    case 20:
        return "Warning: face at line"
                + to_string(lineNumber) + " has already been created (duplicate names)." +
                                          " It can't be initiated.";
    case 21:
        return "Warning: vertex at line"
                + to_string(lineNumber) + " can't be added to face. It has not been created.";

    case 22:
        return "Warning: Face at line"
                + to_string(lineNumber) + " does not complete color definition. " +
                "It can't be instantiated.";
    case 23:
        return "Warning: face at line"
                + to_string(lineNumber) + " can't be added to mesh. It has not been created.";

    }
    return "";
}

void NomeParser::makeWithNome(vector<ParameterBank> &banks,
                                    unordered_map<string, Parameter> &params,
                                    Group &group,
                                    string inputSIF,
                                    vector<string> &banklines,
                                    vector<string> &geometrylines)
{
    banks.clear();
    group.clear();
    ifstream file(inputSIF);
    if (!file.good())
    {
        cout<<"THE PATH OF MINI SIF FILE IS NOT VAILD.";
        exit(1);
    }
    string nextLine;
    int lineNumber = 1;
    bool createBank = false;
    string currentGroup = "";
    unordered_map<string, Parameter>::iterator pIt;
    unordered_map<string, Mesh> meshes;
    unordered_map<string, Mesh>::iterator meshIt;
    unordered_map<string, Group> groups;
    unordered_map<string, Group>::iterator groupIt;
    unordered_map<string, Vertex*> global_vertices;
    unordered_map<string, Vertex*>::iterator vertIt;
    unordered_map<string, PolyLine> polylines;
    unordered_map<string, PolyLine>::iterator lineIt;
    unordered_map<string, Face*> global_faces;
    unordered_map<Face*, vector<Vertex*> > map_face_vertices;
    unordered_map<string, Face*>::iterator faceIt;
    string name = "";
    unordered_map<string, QColor> user_defined_colors;
    unordered_map<string, QColor>::iterator colorIt;
    while(std::getline(file, nextLine))
    {
        istringstream iss(nextLine);
        vector<string> tokens;
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(tokens));
        vector<string>::iterator tIt;
        for(tIt = tokens.begin(); tIt < tokens.end(); tIt++)
        {
            if(testComments(*tIt))
            {
                break;
            }
            else if((*tIt) == "bank")
            {
                banklines.push_back(nextLine);
                ParameterBank newBank;
                if((++tIt) < tokens.end()) {
                    if(!testComments(*tIt))
                    {
                        newBank.setName(QString::fromStdString(*tIt));
                    }
                }
                banks.push_back(newBank);
                createBank = true;
                goto newLineEnd;
            }
            else if((*tIt) == "endbank")
            {
                banklines.push_back(nextLine);
                createBank = false;
                goto newLineEnd;
            }
            else if(createBank && (*tIt) == "set")
            {
                banklines.push_back(nextLine);
                Parameter newParameter;
                int i = 0;
                while(i < 5) {
                    if(tIt >= tokens.end() - 1) {
                        cout<<warning(0, lineNumber)<<endl;
                        goto newLineEnd;
                    }
                    string nextToken = *(++tIt);
                    if(testComments(nextToken)) {
                        cout<<warning(0, lineNumber)<<endl;
                        goto newLineEnd;
                    }
                    switch(i)
                    {
                    case 0:
                        newParameter.name = banks[banks.size() - 1].name
                                + QString::fromStdString("_" + nextToken);
                        name = nextToken;
                        break;
                    case 1:
                        try
                        {
                            newParameter.value = std::stof(nextToken);
                            break;
                        }
                        catch (std::exception e)
                        {
                            cout<<warning(1, lineNumber)<<endl;
                            goto newLineEnd;
                        }
                    case 2:
                        try
                        {
                            newParameter.start = std::stof(nextToken);
                            break;
                        }
                        catch (std::exception e)
                        {
                            cout<<warning(1, lineNumber)<<endl;
                            goto newLineEnd;
                        }
                    case 3:
                        try
                        {
                            newParameter.end = std::stof(nextToken);
                            break;
                        }
                        catch (std::exception e)
                        {
                            cout<<warning(1, lineNumber)<<endl;
                            goto newLineEnd;
                        }
                    case 4:
                        try
                        {
                            newParameter.stepsize = std::stof(nextToken);
                            break;
                        }
                        catch (std::exception e)
                        {
                            cout<<warning(1, lineNumber)<<endl;
                            goto newLineEnd;
                        }
                    }
                    i++;
                }
                pIt = params.find(newParameter.name.toStdString());
                if(pIt == params.end())
                {
                    params[banks[banks.size() - 1].name.toStdString() + "_" + name]
                            = newParameter;
                }
                else
                {
                    cout<<warning(2, lineNumber)<<endl;
                    goto newLineEnd;
                }
                banks[banks.size() - 1].addParameter(
                            &params[banks[banks.size() - 1].name.toStdString() + "_" + name]);
            }
            else if((*tIt) == "surface")
            {
                string color_name;
                QColor new_color;
                if(++tIt < tokens.end())
                {
                    color_name = (*tIt);
                    colorIt = user_defined_colors.find(color_name);
                    if(colorIt != user_defined_colors.end())
                    {
                        cout<<warning(16, lineNumber)<<endl;
                        goto newLineEnd;
                    }
                }
                else
                {
                    cout<<warning(17, lineNumber)<<endl;
                }
                if(++tIt < tokens.end())
                {
                    if((*tIt) != "color")
                    {
                        cout<<warning(17, lineNumber)<<endl;
                    }
                }
                else
                {
                    cout<<warning(17, lineNumber)<<endl;
                }
                string color_expression;
                bool expression_input = false;
                bool inExpression = false;
                while(++tIt < tokens.end())
                {
                    for(char& c : (*tIt))
                    {
                        if(c == '(' && !inExpression)
                        {
                            expression_input = true;
                        }
                        else if(c == ')' && !inExpression)
                        {
                            expression_input = false;
                            goto colordone;
                        }
                        else if(c == '#')
                        {
                            goto newLineEnd;
                        }
                        else if(expression_input)
                        {
                            color_expression.push_back(c);
                        }
                        if(c == '{')
                        {
                            inExpression = true;
                        }
                        else if(c == '}')
                        {
                            inExpression = false;
                        }
                    }
                    color_expression.push_back(' ');
                }
                colordone:
                new_color = evaluate_color_expression(color_expression);
                user_defined_colors[color_name] = new_color;
                goto newLineEnd;
            }
            else if((*tIt) == "funnel")
            {
                geometrylines.push_back(nextLine);
                Mesh newFunnel(1);
                newFunnel.setGlobalParameter(&params);
                if(++tIt < tokens.end())
                {
                    newFunnel.name = (*tIt);
                }
                string funnel_expression;
                bool expression_input = false;
                bool inExpression = false;
                while(++tIt < tokens.end())
                {
                    for(char& c : (*tIt))
                    {
                        if(c == '(' && !inExpression)
                        {
                            expression_input = true;
                        }
                        else if(c == ')' && !inExpression)
                        {
                            expression_input = false;
                        }
                        else if(c == '#')
                        {
                            goto newLineEnd;
                        }
                        else if(expression_input)
                        {
                            funnel_expression.push_back(c);
                        }
                        if(c == '{')
                        {
                            inExpression = true;
                        }
                        else if(c == '}')
                        {
                            inExpression = false;
                        }
                    }
                    funnel_expression.push_back(' ');
                }
                newFunnel.setFunnelParameterValues(funnel_expression);
                newFunnel.makeFunnel();
                if(meshes.find(newFunnel.name) == meshes.end())
                {
                    meshes[newFunnel.name] = newFunnel;
                }
                else
                {
                    cout<<warning(3, lineNumber)<<endl;
                }
                //newFunnel.computeNormals();
            }
            else if((*tIt) == "tunnel")
            {
                geometrylines.push_back(nextLine);
                Mesh newTunnel(2);
                newTunnel.setGlobalParameter(&params);
                if(++tIt < tokens.end())
                {
                    newTunnel.name = (*tIt);
                }
                string tunnel_expression;
                bool expression_input = false;
                bool inExpression = false;
                while(++tIt < tokens.end())
                {
                    for(char& c : (*tIt))
                    {
                        if(c == '(' && !inExpression)
                        {
                            expression_input = true;
                        }
                        else if(c == ')' && !inExpression)
                        {
                            expression_input = false;
                        }
                        else if(c == '#')
                        {
                            goto newLineEnd;
                        }
                        else if(expression_input)
                        {
                            tunnel_expression.push_back(c);
                        }
                        if(c == '{')
                        {
                            inExpression = true;
                        }
                        else if(c == '}')
                        {
                            inExpression = false;
                        }
                    }
                    tunnel_expression.push_back(' ');
                }
                newTunnel.setTunnelParameterValues(tunnel_expression);
                newTunnel.makeTunnel();
                if(meshes.find(newTunnel.name) == meshes.end())
                {
                    meshes[newTunnel.name] = newTunnel;
                }
                else
                {
                    cout<<warning(3, lineNumber)<<endl;
                }
                //newTunnel.computeNormals();
            }
            else if((*tIt) == "object")
            {
                geometrylines.push_back(nextLine);
                Mesh newMesh(0);
                newMesh.setGlobalParameter(&params);
                if(++tIt < tokens.end())
                {
                    if(!testComments(*tIt))
                    {
                        newMesh.name = *tIt;
                    }
                        else
                    {
                        cout<<warning(3, lineNumber)<<endl;
                        goto newLineEnd;
                    }
                }
                string faceInside = "";
                bool addingFace = false;
                while(++tIt < tokens.end())
                {
                    for(char& c : (*tIt))
                    {
                        if(c == '(')
                        {
                            addingFace = true;
                        }
                        else if(c == ')')
                        {
                            addingFace = false;
                            if(faceInside != "")
                            {
                                faceIt = global_faces.find(faceInside);
                                if(faceIt == global_faces.end())
                                {
                                    cout<<warning(23, lineNumber);
                                }
                                else
                                {
                                    Vertex * foundVertex;
                                    vector<Vertex*> vertices;
                                    for(Vertex * vc : map_face_vertices[(faceIt -> second)])
                                    {
                                        foundVertex = NULL;
                                        for(Vertex * v : newMesh.vertList)
                                        {
                                            if(v -> source_vertex == vc)
                                            {
                                                foundVertex = v;
                                                vertices.push_back(v);
                                            }
                                        }
                                        if(foundVertex == NULL)
                                        {
                                            Vertex * newVertex = new Vertex;
                                            newVertex -> isParametric = vc -> isParametric;
                                            newVertex -> position = vc -> position;
                                            newVertex -> ID = newMesh.vertList.size();
                                            newVertex -> source_vertex = vc;
                                            newVertex -> name = vc -> name;
                                            newVertex -> x_expr = vc -> x_expr;
                                            newVertex -> y_expr = vc -> y_expr;
                                            newVertex -> z_expr = vc -> z_expr;
                                            newVertex -> influencingParams = vc -> influencingParams;
                                            newVertex -> params = vc -> params;
                                            newMesh.addVertex(newVertex);
                                            vertices.push_back(newVertex);
                                        }
                                    }
                                    newMesh.addPolygonFace(vertices);
                                    newMesh.faceList[newMesh.faceList.size()-1]->user_defined_color = faceIt -> second -> user_defined_color;
                                    if(faceIt -> second -> user_defined_color)
                                    {
                                        newMesh.faceList[newMesh.faceList.size()-1]->color = faceIt -> second -> color;
                                    }
                                }
                                faceInside = "";
                            }
                            goto endAddingFaceInMesh;
                        }
                        else if(addingFace)
                        {
                            faceInside.push_back(c);
                        }
                    }
                    if(faceInside != "")
                    {
                        faceIt = global_faces.find(faceInside);
                        if(faceIt == global_faces.end())
                        {
                            cout<<warning(23, lineNumber);
                        }
                        else
                        {
                            Vertex * foundVertex;
                            vector<Vertex*> vertices;
                            for(Vertex * vc : map_face_vertices[(faceIt -> second)])
                            {
                                foundVertex = NULL;
                                for(Vertex * v : newMesh.vertList)
                                {
                                    if(v -> source_vertex == vc)
                                    {
                                        foundVertex = v;
                                        vertices.push_back(v);
                                    }
                                }
                                if(foundVertex == NULL)
                                {
                                    Vertex * newVertex = new Vertex;
                                    newVertex -> isParametric = vc -> isParametric;
                                    newVertex -> position = vc -> position;
                                    newVertex -> ID = newMesh.vertList.size();
                                    newVertex -> source_vertex = vc;
                                    newVertex -> name = vc -> name;
                                    newVertex -> x_expr = vc -> x_expr;
                                    newVertex -> y_expr = vc -> y_expr;
                                    newVertex -> z_expr = vc -> z_expr;
                                    newVertex -> influencingParams = vc -> influencingParams;
                                    newVertex -> params = vc -> params;
                                    newMesh.addVertex(newVertex);
                                    vertices.push_back(newVertex);
                                }
                            }
                            newMesh.addPolygonFace(vertices);
                            newMesh.faceList[newMesh.faceList.size()-1]->user_defined_color = faceIt -> second -> user_defined_color;
                            if(faceIt -> second -> user_defined_color)
                            {
                                newMesh.faceList[newMesh.faceList.size()-1]->color = faceIt -> second -> color;
                            }
                        }
                        faceInside = "";
                    }
                }
                endAddingFaceInMesh:
                if(meshes.find(newMesh.name) == meshes.end())
                {
                    //newMesh.computeNormals();
                    meshes[newMesh.name] = newMesh;
                }
                else
                {
                    cout<<warning(3, lineNumber)<<endl;
                }
            }
            else if((*tIt) == "face")
            {
                geometrylines.push_back(nextLine);
                Face * newFace = new Face;
                if((++tIt) < tokens.end()) {
                    if(!testComments(*tIt))
                    {
                        vertIt = global_vertices.find(*tIt);
                        if(vertIt == global_vertices.end())
                        {
                            newFace -> name = *tIt;
                            global_faces[*tIt] = newFace;
                        }
                        else
                        {
                            cout<<warning(20, lineNumber)<<endl;
                        }
                    }
                    else
                    {
                        cout<<warning(19, lineNumber)<<endl;
                    }
                }
                string vertInside = "";
                bool addingVert = false;
                vector<Vertex*> vertices;
                vertices.clear();
                while(++tIt < tokens.end() && (*tIt) != "endface")
                {
                    for(char & c: (*tIt))
                    {
                        if(c == '(')
                        {
                            addingVert = true;
                        }
                        else if(c == ')')
                        {
                            addingVert = false;
                            if(vertInside != "")
                            {
                                vertIt = global_vertices.find(vertInside);
                                if(vertIt == global_vertices.end())
                                {
                                    cout<<warning(21, lineNumber);
                                }
                                else
                                {
                                    vertices.push_back(vertIt -> second);
                                }
                                vertInside = "";
                            }
                            goto endAddingVertInFace;
                        }
                        else if(addingVert)
                        {
                            vertInside.push_back(c);
                        }
                    }
                    if(vertInside != "")
                    {
                        vertIt = global_vertices.find(vertInside);
                        if(vertIt == global_vertices.end())
                        {
                            cout<<warning(21, lineNumber);
                        }
                        else
                        {
                            vertices.push_back(vertIt -> second);
                        }
                        vertInside = "";
                    }
                }
                endAddingVertInFace:
                map_face_vertices[newFace] = vertices;
                if(++tIt < tokens.end() && (*tIt) == "surface")
                {
                    string color_name = "";
                    QColor color;
                    bool foundColor = false;
                    if(++tIt < tokens.end())
                    {
                        color_name = *tIt;
                        colorIt = user_defined_colors.find(color_name);
                        if(colorIt != user_defined_colors.end())
                        {
                            foundColor = true;
                            color = colorIt -> second;
                        }
                        else
                        {
                            cout<<warning(22, lineNumber)<<endl;
                        }
                    }
                    else
                    {
                        cout<<warning(22, lineNumber)<<endl;
                    }
                    if(foundColor)
                    {
                        newFace -> color = color;
                        newFace -> user_defined_color = true;
                    }
                }

            }
            else if((*tIt) == "polyline")
            {
                geometrylines.push_back(nextLine);
                PolyLine newPolyline;
                if((++tIt) < tokens.end() && !testComments(*tIt))
                {
                    lineIt = polylines.find(*tIt);
                    if(lineIt == polylines.end())
                    {
                        newPolyline.name = *tIt;
                    }
                    else
                    {
                        cout<<warning(13, lineNumber)<<endl;
                    }
                }
                else
                {
                    cout<<warning(12, lineNumber)<<endl;
                }
                string vertInside = "";
                bool addingVert = false;
                while(++tIt < tokens.end() && (*tIt) != "endpolyline")
                {
                    for(char& c : (*tIt))
                    {
                        if(c == '(')
                        {
                            addingVert = true;
                        }
                        else if(c == ')')
                        {
                            addingVert = false;
                            if(++tIt < tokens.end() && (*tIt) != "endpolyline")
                            {
                                if(*tIt == "isloop")
                                {
                                    newPolyline.isLoop = true;
                                }
                            }
                            goto endPolyLineWhile;
                        }
                        else if(addingVert)
                        {
                            vertInside.push_back(c);
                        }
                    }
                    if(vertInside != "")
                    {
                        vertIt = global_vertices.find(vertInside);
                        if(vertIt == global_vertices.end())
                        {
                            cout<<warning(14, lineNumber);
                        }
                        else
                        {
                            newPolyline.addVertex(vertIt -> second);
                        }
                        vertInside = "";
                    }
                }
                endPolyLineWhile:
                polylines[newPolyline.name] = newPolyline;
            }
            else if((*tIt) == "point")
            {
                geometrylines.push_back(nextLine);
                Vertex * newVertex = new Vertex;
                if((++tIt) < tokens.end()) {
                    if(!testComments(*tIt))
                    {
                        vertIt = global_vertices.find(*tIt);
                        if(vertIt == global_vertices.end())
                        {
                            newVertex -> name = *tIt;
                            global_vertices[*tIt] = newVertex;
                        }
                        else
                        {
                            cout<<warning(11, lineNumber)<<endl;
                        }
                    }
                    else
                    {
                        cout<<warning(10, lineNumber)<<endl;
                    }
                }
                string xyz;
                bool makingXYZ = false;
                bool inExpression = false;
                while(++tIt < tokens.end() && (*tIt) != "endpoint")
                {
                    for(char& c : (*tIt))
                    {
                        if(c == '(' && !inExpression)
                        {
                            makingXYZ = true;
                        }
                        else if(c == ')' && !inExpression)
                        {
                            makingXYZ = false;
                            goto endPointWhile;
                        }
                        else if(makingXYZ)
                        {
                            xyz.push_back(c);
                            if(c == '{')
                            {
                                inExpression = true;
                            }
                            else if(c == '}')
                            {
                                inExpression = false;
                            }
                        }
                    }
                    if(xyz != "")
                    {
                        xyz.push_back(' ');
                    }
                }
                endPointWhile:
                newVertex->setGlobalParameter(&params);
                newVertex->setVertexParameterValues(xyz);
            }
            else if((*tIt) == "group")
            {
                geometrylines.push_back(nextLine);
                Group newGroup;
                if((++tIt) < tokens.end()) {
                    if(!testComments(*tIt))
                    {
                        newGroup.setName(*tIt);
                    }
                }
                groups[newGroup.name] = newGroup;
                currentGroup = newGroup.name;
                goto newLineEnd;
            }
            else if((*tIt) == "endgroup")
            {
                geometrylines.push_back(nextLine);
                currentGroup = "";
                goto newLineEnd;
            }
            else if((*tIt) == "instance")
            {
                geometrylines.push_back(nextLine);
                string instanceName;
                Mesh newMesh;
                Group newGroup;
                PolyLine newPolyline;
                string className;
                bool findMesh = false;
                bool findGroup = false;
                bool findPolyline = false;
                bool foundColor = false;
                QColor color;
                if((++tIt) < tokens.end()) {
                    if(!testComments(*tIt))
                    {
                        instanceName = *tIt;
                    }
                } else {
                    cout<<warning(5, lineNumber);
                }
                if((++tIt) < tokens.end()) {
                    if(!testComments(*tIt))
                    {
                        className = *tIt;
                    }
                } else {
                    cout<<warning(6, lineNumber);
                }
                if(className == "consolidatemesh")
                {
                    goto newLineEnd;
                }
                meshIt = meshes.find(className);
                if(meshIt != meshes.end())
                {
                    newMesh = (meshIt -> second).makeCopy(instanceName);
                    findMesh = true;
                }
                else
                {
                    groupIt = groups.find(className);
                    if(groupIt != groups.end())
                    {
                        newGroup = (groupIt -> second).makeCopy(instanceName);
                        findGroup = true;
                    }
                    else
                    {
                        lineIt = polylines.find(className);
                        if(lineIt != polylines.end())
                        {
                            newPolyline = (lineIt -> second).makeCopy(instanceName);
                            findPolyline = true;
                        }
                        else
                        {
                            cout<<warning(5, lineNumber);
                        }
                    }
                }
                vector<Transformation> transformations_up;
                while(++tIt < tokens.end() && (*tIt) != "endinstance")
                {
                    if(testComments(*tIt))
                    {
                        goto newLineEnd;
                    }
                    if(*tIt == "rotate")
                    {
                        string xyz;
                        string angle;
                        bool makingXYZ = false;
                        bool makingAngle = false;
                        bool doneXYZ = false;
                        bool inExpression = false;
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(' && !inExpression)
                                {
                                    if(!doneXYZ)
                                    {
                                        makingXYZ = true;
                                    }
                                    else
                                    {
                                        makingAngle = true;
                                    }
                                }
                                else if(c == ')' && !inExpression)
                                {
                                    if(makingXYZ)
                                    {
                                        doneXYZ = true;
                                        makingXYZ = false;
                                    } else if(makingAngle)
                                    {
                                        makingAngle = false;
                                        goto endWhile1;
                                    }
                                }
                                else
                                {
                                    if(makingXYZ)
                                    {
                                        xyz.push_back(c);
                                        if(c == '{')
                                        {
                                            inExpression = true;
                                        }
                                        else if(c == '}')
                                        {
                                            inExpression = false;
                                        }
                                    }
                                    else if(makingAngle)
                                    {
                                        angle.push_back(c);
                                        if(c == '{')
                                        {
                                            inExpression = true;
                                        }
                                        else if(c == '}')
                                        {
                                            inExpression = false;
                                        }
                                    }
                                }
                            }
                            if(makingXYZ && xyz != "")
                            {
                                xyz.push_back(' ');
                            }
                            else if(makingAngle && angle != "")
                            {
                                angle.push_back(' ');
                            }
                        }
                        endWhile1:
                        Transformation t(1, &params, xyz, angle);
                        transformations_up.push_back(t);
                    }
                    else if(*tIt == "translate" || *tIt == "scale")
                    {
                        bool isTranslate = false;
                        if(*tIt == "translate")
                        {
                            isTranslate = true;
                        }
                        string xyz = "";
                        bool makingXYZ = false;
                        bool inExpression = false;
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(' && !inExpression)
                                {
                                    makingXYZ = true;
                                }
                                else if(c == ')' && !inExpression)
                                {
                                    makingXYZ = false;
                                    goto endWhile2;
                                }
                                else if(makingXYZ)
                                {
                                    xyz.push_back(c);
                                    if(c == '{')
                                    {
                                        inExpression = true;
                                    }
                                    else if(c == '}')
                                    {
                                        inExpression = false;
                                    }
                                }
                            }
                            if(xyz != "")
                            {
                                xyz.push_back(' ');
                            }
                        }
                        endWhile2:
                        if(isTranslate)
                        {
                            Transformation t(3,&params, xyz);
                            transformations_up.push_back(t);
                        }
                        else
                        {
                            Transformation t(2, &params, xyz);
                            transformations_up.push_back(t);
                        }
                    }
                    else if(*tIt == "mirror")
                    {
                        string xyzw = "";
                        bool makingXYZW = false;
                        bool inExpression = false;
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(' && !inExpression)
                                {
                                    makingXYZW = true;
                                }
                                else if(c == ')' && !inExpression)
                                {
                                    makingXYZW = false;
                                    goto endWhile3;
                                }
                                else if(makingXYZW)
                                {
                                    xyzw.push_back(c);
                                    if(c == '{')
                                    {
                                        inExpression = true;
                                    }
                                    else if(c == '}')
                                    {
                                        inExpression = false;
                                    }
                                }
                            }
                            if(xyzw != "")
                            {
                                xyzw.push_back(' ');
                            }
                        }
                        endWhile3:
                        Transformation t(4, &params, xyzw);
                        transformations_up.push_back(t);
                    }
                    if(*tIt == "surface")
                    {
                        string color_name;
                        if(++tIt < tokens.end())
                        {
                            color_name = *tIt;
                            colorIt = user_defined_colors.find(color_name);
                            if(colorIt != user_defined_colors.end())
                            {
                                color = colorIt -> second;
                                foundColor = true;
                            }
                            else
                            {
                                cout<<warning(18, lineNumber)<<endl;
                            }
                        }
                        else
                        {
                            cout<<warning(18, lineNumber)<<endl;
                        }
                    }
                }
                if(currentGroup != "")
                {
                    if(findMesh)
                    {
                        newMesh.setTransformation(transformations_up);
                        if(foundColor)
                        {
                            newMesh.setColor(color);
                            newMesh.user_set_color = true;
                        }
                        groups[currentGroup].addMesh(newMesh);
                    }
                    else if(findGroup)
                    {
                        newGroup.setTransformation(transformations_up);
                        if(foundColor)
                        {
                            newGroup.setColor(color);
                            newGroup.user_set_color = true;
                        }
                        groups[currentGroup].addGroup(newGroup);
                    }
                    else if(findPolyline)
                    {
                        newPolyline.setTransformation(transformations_up);
                        if(foundColor)
                        {
                            newPolyline.setColor(color);
                            newPolyline.user_set_color = true;
                        }
                        groups[currentGroup].addPolyline(newPolyline);
                    }
                }
                else
                {
                    if(findMesh)
                    {
                        newMesh.setTransformation(transformations_up);
                        if(foundColor)
                        {
                            newMesh.setColor(color);
                            newMesh.user_set_color = true;
                        }
                        group.addMesh(newMesh);
                    }
                    else if(findGroup)
                    {
                        newGroup.setTransformation(transformations_up);
                        if(foundColor)
                        {
                            newGroup.setColor(color);
                            newGroup.user_set_color = true;
                        }
                        group.addGroup(newGroup);
                    }
                    else if(findPolyline)
                    {
                        newPolyline.setTransformation(transformations_up);
                        if(foundColor)
                        {
                            newPolyline.setColor(color);
                            newPolyline.user_set_color = true;
                        }
                        group.addPolyline(newPolyline);
                    }
                }
            }
        }
        newLineEnd:
        lineNumber++;
    }
    group.mapFromParameters();
}

void NomeParser::appendWithANOM(unordered_map<string, Parameter> &params,
                                   Group &group,
                                   SlideGLWidget* canvas,
                                   string inputSIF)
{
    ifstream file(inputSIF);
    if (!file.good())
    {
        cout<<"THE PATH OF MINI SIF FILE IS NOT VAILD.";
        exit(1);
    }
    string nextLine;
    int lineNumber = 1;
    bool restoreBank = false;
    bool restoreConsolidateMesh = false;
    unordered_map<string, Parameter>::iterator pIt;
    vector<Vertex*> restore_vertices;
    unordered_map<string, Group> groups;
    string currentGroup = "";
    while(std::getline(file, nextLine))
    {
        istringstream iss(nextLine);
        vector<string> tokens;
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(tokens));
        vector<string>::iterator tIt;
        for(tIt = tokens.begin(); tIt < tokens.end(); tIt++)
        {
            if(testComments(*tIt))
            {
                break;
            }
            else if((*tIt) == "savedparameter")
            {
                restoreBank = true;
                goto newLineEnd;
            }
            else if((*tIt) == "endsavedparameter")
            {
                restoreBank = false;
                goto newLineEnd;
            }
            else if((*tIt) == "consolidate")
            {
                restoreConsolidateMesh = true;
                goto newLineEnd;
            }
            else if((*tIt) == "endconsolidate")
            {
                restoreConsolidateMesh = false;
                goto newLineEnd;
            }
            else if(restoreBank)
            {
                pIt = params.find(*tIt);
                if(pIt == params.end())
                {
                    cout<<warning(7, lineNumber);
                }
                else
                {
                    tIt++;
                    if(tIt >= tokens.end() || testComments(*tIt))
                    {
                        cout<<warning(8, lineNumber);
                    }
                    else
                    {
                        float value = stof(*tIt);
                        (pIt -> second).changeParameterValue(value);
                    }
                }
                goto newLineEnd;
            }
            else if(restoreConsolidateMesh)
            {
                if(*tIt == "consolidateface")
                {
                    restore_vertices.clear();
                    goto newLineEnd;
                }
                else if(*tIt == "endconsolidateface")
                {
                    canvas->consolidate_mesh.addPolygonFace(restore_vertices);
                    goto newLineEnd;
                }
                else if(*tIt == "vertex")
                {
                    tIt++;
                    if(tIt >= tokens.end() || testComments(*tIt))
                    {
                        cout<<warning(9, lineNumber);
                    }
                    Vertex *v = (canvas -> hierarchical_scene_transformed).findVertexInThisGroup(*tIt);
                    if(!(canvas -> master_mesh.isEmpty())) /* Dealing with recovery of SIF.*/
                    {
                        v = (canvas -> master_mesh.findVertexInThisMesh(*tIt));
                    }
                    if(v == NULL)
                    {
                        cout<<warning(9, lineNumber);
                    }
                    else
                    {
                        (canvas -> consolidate_mesh).addVertex(v);
                        restore_vertices.push_back(v);
                    }
                    goto newLineEnd;
                }
            }
            else if((*tIt) == "instance")
            {
                string instanceName;
                Mesh newMesh;
                string className;
                if((++tIt) < tokens.end()) {
                    if(!testComments(*tIt))
                    {
                        instanceName = *tIt;
                    }
                }
                else
                {
                    cout<<warning(5, lineNumber);
                }
                if((++tIt) < tokens.end()) {
                    if(!testComments(*tIt))
                    {
                        className = *tIt;
                    }
                }
                else
                {
                    cout<<warning(6, lineNumber);
                }
                if(className == "consolidatemesh")
                {
                    newMesh = (canvas -> consolidate_mesh).makeCopyForTempMesh(instanceName);
                }
                else
                {
                    goto newLineEnd;
                }
                vector<Transformation> transformations_up;
                while(++tIt < tokens.end() && (*tIt) != "endinstance")
                {
                    if(testComments(*tIt))
                    {
                        goto newLineEnd;
                    }
                    if(*tIt == "rotate")
                    {
                        string xyz;
                        string angle;
                        bool makingXYZ = false;
                        bool makingAngle = false;
                        bool doneXYZ = false;
                        bool inExpression = false;
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(' && !inExpression)
                                {
                                    if(!doneXYZ)
                                    {
                                        makingXYZ = true;
                                    }
                                    else
                                    {
                                        makingAngle = true;
                                    }
                                }
                                else if(c == ')' && !inExpression)
                                {
                                    if(makingXYZ)
                                    {
                                        doneXYZ = true;
                                        makingXYZ = false;
                                    } else if(makingAngle)
                                    {
                                        makingAngle = false;
                                        goto endWhile1;
                                    }
                                }
                                else
                                {
                                    if(makingXYZ)
                                    {
                                        xyz.push_back(c);
                                        if(c == '{')
                                        {
                                            inExpression = true;
                                        }
                                        else if(c == '}')
                                        {
                                            inExpression = false;
                                        }
                                    }
                                    else if(makingAngle)
                                    {
                                        angle.push_back(c);
                                        if(c == '{')
                                        {
                                            inExpression = true;
                                        }
                                        else if(c == '}')
                                        {
                                            inExpression = false;
                                        }
                                    }
                                }
                            }
                            if(makingXYZ && xyz != "")
                            {
                                xyz.push_back(' ');
                            }
                            else if(makingAngle && angle != "")
                            {
                                angle.push_back(' ');
                            }
                        }
                        endWhile1:
                        Transformation t(1, &params, xyz, angle);
                        transformations_up.push_back(t);
                    }
                    else if(*tIt == "translate" || *tIt == "scale")
                    {
                        bool isTranslate = false;
                        if(*tIt == "translate")
                        {
                            isTranslate = true;
                        }
                        string xyz = "";
                        bool makingXYZ = false;
                        bool inExpression = false;
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(' && !inExpression)
                                {
                                    makingXYZ = true;
                                }
                                else if(c == ')' && !inExpression)
                                {
                                    makingXYZ = false;
                                    goto endWhile2;
                                }
                                else if(makingXYZ)
                                {
                                    xyz.push_back(c);
                                    if(c == '{')
                                    {
                                        inExpression = true;
                                    }
                                    else if(c == '}')
                                    {
                                        inExpression = false;
                                    }
                                }
                            }
                            if(xyz != "")
                            {
                                xyz.push_back(' ');
                            }
                        }
                        endWhile2:
                        if(isTranslate)
                        {
                            Transformation t(3,&params, xyz);
                            transformations_up.push_back(t);
                        }
                        else
                        {
                            Transformation t(2, &params, xyz);
                            transformations_up.push_back(t);
                        }
                    }
                    else if(*tIt == "mirror")
                    {
                        string xyzw = "";
                        bool makingXYZW = false;
                        bool inExpression = false;
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(' && !inExpression)
                                {
                                    makingXYZW = true;
                                }
                                else if(c == ')' && !inExpression)
                                {
                                    makingXYZW = false;
                                    goto endWhile3;
                                }
                                else if(makingXYZW)
                                {
                                    xyzw.push_back(c);
                                    if(c == '{')
                                    {
                                        inExpression = true;
                                    }
                                    else if(c == '}')
                                    {
                                        inExpression = false;
                                    }
                                }
                            }
                            if(xyzw != "")
                            {
                                xyzw.push_back(' ');
                            }
                        }
                        endWhile3:
                        Transformation t(4, &params, xyzw);
                        transformations_up.push_back(t);
                    }
                }
                if(currentGroup != "")
                {
                    newMesh.setTransformation(transformations_up);
                    groups[currentGroup].addMesh(newMesh);
                }
                else
                {
                    newMesh.setTransformation(transformations_up);
                    group.addMesh(newMesh);
                }
            }
        }
        newLineEnd:
        lineNumber++;
    }
    canvas -> updateFromSavedMesh();
}
