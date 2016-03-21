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
                + "contains string that can't be parsed, skipping this line.";
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
    while(std::getline(file, nextLine))
    {
        cout<<"Line "<<lineNumber<<endl;
        //std::cout<<nextLine<<endl;
        istringstream iss(nextLine);
        vector<string> tokens;
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(tokens));
        vector<string>::iterator tIt;
        bool createParam = false;
        for(tIt = tokens.begin(); tIt < tokens.end(); tIt++)
        {
            if(testComments(*tIt))
            {
                break;
            }
            else if((*tIt) == "bank")
            {
                ParameterBank newBank;
                banks.push_back(newBank);
                createBank = true;
                cout<<"Find a bank!"<<endl;
            }
            else if((*tIt) == "endbank")
            {
                createBank = false;
                cout<<"Finish creating this bank."<<endl;
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
                            cout<<"Hmm"<<endl;
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
                banks[banks.size() - 1].addParameter(newParameter);
            }
            cout<<*tIt<<" ";
        }
        newLineEnd:
        lineNumber++;
    }
}
