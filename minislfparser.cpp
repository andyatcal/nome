#include "minislfparser.h"

MiniSlfParser::MiniSlfParser()
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
                + to_string(lineNumber) + "does not have a retored value.";
    case 9:
        return "Warning: vertex at line"
                + to_string(lineNumber) + "can't be restored.";
    }
    return "";
}

void MiniSlfParser::makeWithMiniSLF(vector<ParameterBank> &banks,
                                    unordered_map<string, Parameter> &params,
                                    Group &group, string inputSIF)
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
    string name = "";
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
                createBank = false;
                goto newLineEnd;
            }
            else if(createBank && (*tIt) == "set")
            {
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
                        newParameter.name = banks[banks.size() - 1].name + QString::fromStdString("_" + nextToken);
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
                    params[banks[banks.size() - 1].name.toStdString() + "_" + name] = newParameter;
                }
                else
                {
                    cout<<warning(2, lineNumber)<<endl;
                    goto newLineEnd;
                }
                banks[banks.size() - 1].addParameter(&params[banks[banks.size() - 1].name.toStdString() + "_" + name]);
            }
            else if((*tIt) == "funnel")
            {
                Mesh newFunnel(1);
                newFunnel.setGlobalParameter(&params);
                if(++tIt < tokens.end())
                {
                    newFunnel.name = (*tIt);
                }
                string funnel_expression;
                bool expression_input = false;
                while(++tIt < tokens.end())
                {
                    for(char& c : (*tIt))
                    {
                        if(c == '(')
                        {
                            expression_input = true;
                        }
                        else if(c == ')')
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
                newFunnel.computeNormals();
                //group.addMesh(newFunnel);
            }
            else if((*tIt) == "tunnel")
            {
                Mesh newTunnel(2);
                newTunnel.setGlobalParameter(&params);
                if(++tIt < tokens.end())
                {
                    newTunnel.name = (*tIt);
                }
                string tunnel_expression;
                bool expression_input = false;
                while(++tIt < tokens.end())
                {
                    for(char& c : (*tIt))
                    {
                        if(c == '(')
                        {
                            expression_input = true;
                        }
                        else if(c == ')')
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
                newTunnel.computeNormals();
            }
            else if((*tIt) == "group")
            {
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
                currentGroup = "";
                goto newLineEnd;
            }
            else if((*tIt) == "instance")
            {
                string instanceName;
                Mesh newMesh;
                Group newGroup;
                string newInstanceName;
                bool findMesh = false;
                bool findGroup = false;
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
                        newInstanceName = *tIt;
                    }
                } else {
                    cout<<warning(6, lineNumber);
                }
                meshIt = meshes.find(instanceName);
                if(meshIt != meshes.end())
                {
                    newMesh = (meshIt -> second).makeCopy(newInstanceName);
                    findMesh = true;
                }
                else
                {
                    groupIt = groups.find(instanceName);
                    if(groupIt != groups.end())
                    {
                        newGroup = (groupIt -> second).makeCopy(newInstanceName);
                        findGroup = true;
                    }
                    else
                    {
                        cout<<warning(5, lineNumber);
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
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(')
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
                                else if(c == ')')
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
                                    }
                                    else if(makingAngle)
                                    {
                                        angle.push_back(c);
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
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(')
                                {
                                    makingXYZ = true;
                                }
                                else if(c == ')')
                                {
                                    makingXYZ = false;
                                    goto endWhile2;
                                }
                                else if(makingXYZ)
                                {
                                    xyz.push_back(c);
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
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(')
                                {
                                    makingXYZW = true;
                                }
                                else if(c == ')')
                                {
                                    makingXYZW = false;
                                    goto endWhile3;
                                }
                                else if(makingXYZW)
                                {
                                    xyzw.push_back(c);
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
                    if(findMesh)
                    {
                        newMesh.setTransformation(transformations_up);
                        groups[currentGroup].addMesh(newMesh);

                    } else if(findGroup)
                    {
                        newGroup.setTransformation(transformations_up);
                        groups[currentGroup].addGroup(newGroup);
                    }
                }
                else
                {
                    if(findMesh)
                    {
                        newMesh.setTransformation(transformations_up);
                        group.addMesh(newMesh);
                    }
                    else
                    {
                        newGroup.setTransformation(transformations_up);
                        group.addGroup(newGroup);
                    }
                }
            }
        }
        newLineEnd:
        lineNumber++;
    }
    group.mapFromParameters();
}

void MiniSlfParser::appendWithASLF(vector<ParameterBank> &banks,
                                   unordered_map<string, Parameter> &params,
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
    bool restoreWorkingMesh = false;
    unordered_map<string, Parameter>::iterator pIt;
    vector<Vertex*> vertices;
    unordered_map<string, Group> groups;
    unordered_map<string, Group>::iterator groupIt;
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
            else if((*tIt) == "savedworkingmesh")
            {
                restoreWorkingMesh = true;
                goto newLineEnd;
            }
            else if((*tIt) == "endsavedworkingmesh")
            {
                restoreWorkingMesh = false;
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
            else if(restoreWorkingMesh)
            {
                if(*tIt == "face")
                {
                    vertices.clear();
                    goto newLineEnd;
                }
                else if(*tIt == "endface")
                {
                    canvas->temp_mesh.addPolygonFace(vertices);
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
                    if(v == NULL)
                    {
                        cout<<warning(9, lineNumber);
                    }
                    else
                    {
                        vertices.push_back(v);
                    }
                    goto newLineEnd;
                }
                else if((*tIt) == "group")
                {
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
                    currentGroup = "";
                    goto newLineEnd;
                }
                else if((*tIt) == "instance")
                {
                    string instanceName;
                    Mesh newMesh;
                    Group newGroup;
                    string newInstanceName;
                    bool findMesh = false;
                    bool findGroup = false;
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
                            newInstanceName = *tIt;
                        }
                    }
                    else
                    {
                        cout<<warning(6, lineNumber);
                    }
                    if(instanceName == "tempmesh")
                    {
                        newMesh = (canvas -> temp_mesh).makeCopy(newInstanceName);
                        findMesh = true;
                    }
                    else
                    {
                        groupIt = groups.find(instanceName);
                        if(groupIt != groups.end())
                        {
                            newGroup = (groupIt -> second).makeCopy(newInstanceName);
                            findGroup = true;
                        }
                        else
                        {
                            cout<<warning(5, lineNumber);
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
                            while(++tIt < tokens.end() && (*tIt) != "endinstance")
                            {
                                for(char& c : (*tIt))
                                {
                                    if(c == '(')
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
                                    else if(c == ')')
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
                                        }
                                        else if(makingAngle)
                                        {
                                            angle.push_back(c);
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
                            while(++tIt < tokens.end() && (*tIt) != "endinstance")
                            {
                                for(char& c : (*tIt))
                                {
                                    if(c == '(')
                                    {
                                        makingXYZ = true;
                                    }
                                    else if(c == ')')
                                    {
                                        makingXYZ = false;
                                        goto endWhile2;
                                    }
                                    else if(makingXYZ)
                                    {
                                        xyz.push_back(c);
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
                            while(++tIt < tokens.end() && (*tIt) != "endinstance")
                            {
                                for(char& c : (*tIt))
                                {
                                    if(c == '(')
                                    {
                                        makingXYZW = true;
                                    }
                                    else if(c == ')')
                                    {
                                        makingXYZW = false;
                                        goto endWhile3;
                                    }
                                    else if(makingXYZW)
                                    {
                                        xyzw.push_back(c);
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
                        if(findMesh)
                        {
                            newMesh.setTransformation(transformations_up);
                            groups[currentGroup].addMesh(newMesh);

                        } else if(findGroup)
                        {
                            newGroup.setTransformation(transformations_up);
                            groups[currentGroup].addGroup(newGroup);
                        }
                    }
                    else
                    {
                        if(findMesh)
                        {
                            newMesh.setTransformation(transformations_up);
                            group.addMesh(newMesh);
                        }
                        else
                        {
                            newGroup.setTransformation(transformations_up);
                            group.addGroup(newGroup);
                        }
                    }
                }
            }
        }
        newLineEnd:
        lineNumber++;
    }
    canvas -> updateFromSavedTempMesh();
}
