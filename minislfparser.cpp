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
    }
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
    unordered_map<string, Parameter> params;
    unordered_map<string, Parameter>::iterator pIt;
    unordered_map<string, Mesh> meshes;
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
                newFunnel.setColor(QColor(0, 255, 0));
                group.addMesh(newFunnel);
                newFunnel.computeNormals();
            }
        }
        newLineEnd:
        lineNumber++;
    }
}
