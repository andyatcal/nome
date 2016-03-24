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
    }
    return "";
}

void MiniSlfParser::makeWithMiniSLF(vector<ParameterBank> &banks, Group &group, string inputSIF)
{
    banks.clear();
    group.clear();
    ifstream file(inputSIF);
    if (!file.good()) {
        cout<<"THE PATH OF MINI SIF FILE IS NOT VAILD.";
        exit(1);
    }
    string nextLine;
    int lineNumber = 1;
    bool createBank = false;
    string currentGroup = "";
    unordered_map<string, Parameter> params;
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
                while(i < 6) {
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
                        newParameter.name = QString::fromStdString(nextToken);
                        name = nextToken;
                        break;
                    case 1:
                        try
                        {
                            newParameter.start = std::stoi(nextToken);
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
                            newParameter.end = std::stoi(nextToken);
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
                            newParameter.value = std::stoi(nextToken);
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
                            newParameter.stepsize = std::stoi(nextToken);
                            break;
                        }
                        catch (std::exception e)
                        {
                            cout<<warning(1, lineNumber)<<endl;
                            goto newLineEnd;
                        }
                    case 5:
                        try
                        {
                            newParameter.multiplier = std::stof(nextToken);
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
                    params[name] = newParameter;
                }
                else
                {
                    cout<<warning(2, lineNumber)<<endl;
                    goto newLineEnd;
                }
                banks[banks.size() - 1].addParameter(newParameter);
            }
            else if((*tIt) == "funnel")
            {
                Funnel newFunnel;
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
                newFunnel.setParameterValues(funnel_expression);
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
                meshIt = meshes.find(instanceName);
                if(meshIt != meshes.end())
                {
                    newMesh = meshCopy(meshIt -> second);
                    findMesh = true;
                }
                else
                {
                    groupIt = groups.find(instanceName);
                    if(groupIt != groups.end())
                    {
                        newGroup = (groupIt -> second).makeCopy();
                        findGroup = true;
                    }
                    else
                    {
                        cout<<warning(5, lineNumber);
                    }
                }
                vector<mat4> transformations_up;
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
                        vec3 axis;
                        float radian;
                        bool makingXYZ = false;
                        bool makingAngle = false;
                        while(++tIt < tokens.end() && (*tIt) != "endinstance")
                        {
                            for(char& c : (*tIt))
                            {
                                if(c == '(')
                                {
                                    if(!makingXYZ)
                                    {
                                        makingXYZ = true;
                                    }
                                    else if(!makingAngle)
                                    {
                                        makingAngle = true;
                                        makingXYZ = false;
                                    }
                                }
                                else if(c == ')' && makingXYZ)
                                {
                                    axis = getXYZ(xyz);
                                }
                                else if(c == ')' && makingAngle)
                                {
                                    radian = getOneValue(angle) / 180 * PI;
                                    goto endWhile1;
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
                            endWhile1:
                            if(makingXYZ && xyz != "")
                            {
                                xyz.push_back(' ');
                            }
                            else if(makingAngle && angle != "")
                            {
                                angle.push_back(' ');
                            }
                        }
                        transformations_up.push_back(krotate(axis, radian));
                    }
                    else if(*tIt == "translate" || *tIt == "scale")
                    {
                        bool isTranslate = false;
                        if(*tIt == "translate")
                        {
                            isTranslate = true;
                        }
                        string xyz = "";
                        vec3 v;
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
                                    v = getXYZ(xyz);
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
                            transformations_up.push_back(ktranslate(v));
                        }
                        else
                        {
                            transformations_up.push_back(kscale(v));
                        }
                    }
                    else if(*tIt == "mirror")
                    {
                        string xyzw = "";
                        vec4 v;
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
                                    v = getXYZW(xyzw);
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
                        transformations_up.push_back(kmirror(v));
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
                        group.addMesh(newMesh);
                    }
                    else
                    {
                        group.addGroup(newGroup);
                    }
                }
            }
        }
        newLineEnd:
        lineNumber++;
    }
    group.setColor(QColor(0, 255, 0));
    group.assignColor();
}
