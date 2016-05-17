#include "utils.h"
#include "transformation.h"
#include "mesh.h"
float evaluate_expression(string expr, unordered_map<string, Parameter> *params)
{
    //cout<<expr<<endl;
    vector<string> tokens;
    stack<string> numberStack;
    string number = "";
    string lastNumber = "";
    bool readingFromExpr = false;
    string parameterName = "";
    for(char& m : expr)
    {
        if(!readingFromExpr && ((m >= '0' && m <= '9') || m == '.'))
        {
            number.push_back(m);
        }
        else if(m == '$')
        {
            readingFromExpr = true;
        }
        else if(readingFromExpr && !(m == ' ' || m == '(' || m ==')'
                                     || isOperator(m)))
        {
            parameterName.push_back(m);
        }
        else
        {
            if(number != "")
            {
                tokens.push_back(number);
                lastNumber = number;
                number = "";
            }
            else if(parameterName != "")
            {
                string value = to_string(getParameterValue(parameterName, params));
                tokens.push_back(value);
                lastNumber = value;
                parameterName = "";
                readingFromExpr = false;
            }
            if(isOperator(m))
            {
                if(lastNumber == "")
                {
                    tokens.push_back("0");
                }
                while(numberStack.size() > 0)
                {
                    string last = numberStack.top();
                    if(getPriority(last[0]) >= getPriority(m))
                    {
                        tokens.push_back(last);
                        numberStack.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                string newString = "";
                newString.push_back(m);
                numberStack.push(newString);
            }
            else if(m == '(')
            {
                string newString = "";
                newString.push_back(m);
                numberStack.push(newString);
                lastNumber = "";
            }
            else if(m == ')')
            {
                while(numberStack.size() > 0) {
                    string last = numberStack.top();
                    numberStack.pop();
                    if(last != "(")
                    {
                        tokens.push_back(last);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    if(number != "")
    {
        tokens.push_back(number);
    } else if(parameterName != "")
    {
        string value = to_string(getParameterValue(parameterName, params));
        tokens.push_back(value);
        lastNumber = value;
    }
    while(!numberStack.empty())
    {
        tokens.push_back(numberStack.top());
        numberStack.pop();
    }
    stack<float> operands;
    vector<string>::iterator tIt;
    for(tIt = tokens.begin(); tIt < tokens.end(); tIt++)
    {
        string token = (*tIt);
        if(token.size() == 1 && isOperator(token[0]))
        {
            float y = operands.top();
            operands.pop();
            float x = operands.top();
            operands.pop();
            operands.push(getVal(x, y, token[0]));
        }
        else
        {
            operands.push(stof(token));
        }
    }
    if(operands.empty())
    {
        return 0.0f;
    }
    return operands.top();
}

float evaluate_mesh_expression(string expr, unordered_map<string, Parameter> *params, Mesh * mesh)
{
    //cout<<expr<<endl;
    vector<string> tokens;
    stack<string> numberStack;
    string number = "";
    string lastNumber = "";
    bool readingFromExpr = false;
    string parameterName = "";
    for(char& m : expr)
    {
        if(!readingFromExpr && ((m >= '0' && m <= '9') || m == '.'))
        {
            number.push_back(m);
        } else if(m == '$')
        {
            readingFromExpr = true;
        }
        else if(readingFromExpr && !(m == ' ' || m == '(' || m ==')' || isOperator(m)))
        {
            parameterName.push_back(m);
        }
        else
        {
            if(number != "")
            {
                tokens.push_back(number);
                lastNumber = number;
                number = "";
            } else if(parameterName != "")
            {
                string value = to_string(getMeshParameterValue(parameterName, params, mesh));
                tokens.push_back(value);
                lastNumber = value;
                parameterName = "";
                readingFromExpr = false;
            }
            if(isOperator(m))
            {
                if(lastNumber == "") {
                    tokens.push_back("0");
                }
                while(numberStack.size() > 0)
                {
                    string last = numberStack.top();
                    if(getPriority(last[0]) >= getPriority(m))
                    {
                        tokens.push_back(last);
                        numberStack.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                string newString = "";
                newString.push_back(m);
                numberStack.push(newString);
            }
            else if(m == '(')
            {
                string newString = "";
                newString.push_back(m);
                numberStack.push(newString);
                lastNumber = "";
            }
            else if(m == ')')
            {
                while(numberStack.size() > 0) {
                    string last = numberStack.top();
                    numberStack.pop();
                    if(last != "(")
                    {
                        tokens.push_back(last);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    if(number != "")
    {
        tokens.push_back(number);
    } else if(parameterName != "")
    {
        string value = to_string(getMeshParameterValue(parameterName, params, mesh));
        tokens.push_back(value);
        lastNumber = value;
    }
    while(!numberStack.empty())
    {
        tokens.push_back(numberStack.top());
        numberStack.pop();
    }
    stack<float> operands;
    vector<string>::iterator tIt;
    for(tIt = tokens.begin(); tIt < tokens.end(); tIt++)
    {
        string token = (*tIt);
        if(token.size() == 1 && isOperator(token[0]))
        {
            float y = operands.top();
            operands.pop();
            float x = operands.top();
            operands.pop();
            operands.push(getVal(x, y, token[0]));
        }
        else
        {
            operands.push(stof(token));
        }
    }
    if(operands.empty())
    {
        return 0.0f;
    }
    return operands.top();
}

float evaluate_transformation_expression(string expr,
                                         unordered_map<string, Parameter> *params,
                                         Transformation * t)
{
    //cout<<expr<<endl;
    vector<string> tokens;
    stack<string> numberStack;
    string number = "";
    string lastNumber = "";
    bool readingFromExpr = false;
    string parameterName = "";
    for(char& m : expr)
    {
        if(!readingFromExpr && ((m >= '0' && m <= '9') || m == '.'))
        {
            number.push_back(m);
        } else if(m == '$')
        {
            readingFromExpr = true;
        }
        else if(readingFromExpr && !(m == ' ' || m == '(' || m ==')' || isOperator(m)))
        {
            parameterName.push_back(m);
        }
        else
        {
            if(number != "")
            {
                tokens.push_back(number);
                lastNumber = number;
                number = "";
            } else if(parameterName != "")
            {
                string value = to_string(getTransformationParameterValue(parameterName, params, t));
                tokens.push_back(value);
                lastNumber = value;
                parameterName = "";
                readingFromExpr = false;
            }
            if(isOperator(m))
            {
                if(lastNumber == "") {
                    tokens.push_back("0");
                }
                while(numberStack.size() > 0)
                {
                    string last = numberStack.top();
                    if(getPriority(last[0]) >= getPriority(m))
                    {
                        tokens.push_back(last);
                        numberStack.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                string newString = "";
                newString.push_back(m);
                numberStack.push(newString);
            }
            else if(m == '(')
            {
                string newString = "";
                newString.push_back(m);
                numberStack.push(newString);
                lastNumber = "";
            }
            else if(m == ')')
            {
                while(numberStack.size() > 0) {
                    string last = numberStack.top();
                    numberStack.pop();
                    if(last != "(")
                    {
                        tokens.push_back(last);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    if(number != "")
    {
        tokens.push_back(number);
    } else if(parameterName != "")
    {
        string value = to_string(getTransformationParameterValue(parameterName, params, t));
        tokens.push_back(value);
        lastNumber = value;
    }
    while(!numberStack.empty())
    {
        tokens.push_back(numberStack.top());
        numberStack.pop();
    }
    stack<float> operands;
    vector<string>::iterator tIt;
    for(tIt = tokens.begin(); tIt < tokens.end(); tIt++)
    {
        string token = (*tIt);
        if(token.size() == 1 && isOperator(token[0]))
        {
            float y = operands.top();
            operands.pop();
            float x = operands.top();
            operands.pop();
            operands.push(getVal(x, y, token[0]));
        }
        else
        {
            operands.push(stof(token));
        }
    }
    if(operands.empty())
    {
        return 0.0f;
    }
    return operands.top();
}

float evaluate_vertex_expression(string expr,
                                         unordered_map<string, Parameter> *params,
                                         Vertex * v)
{
    //cout<<expr<<endl;
    vector<string> tokens;
    stack<string> numberStack;
    string number = "";
    string lastNumber = "";
    bool readingFromExpr = false;
    string parameterName = "";
    for(char& m : expr)
    {
        if(!readingFromExpr && ((m >= '0' && m <= '9') || m == '.'))
        {
            number.push_back(m);
        } else if(m == '$')
        {
            readingFromExpr = true;
        }
        else if(readingFromExpr && !(m == ' ' || m == '(' || m ==')' || isOperator(m)))
        {
            parameterName.push_back(m);
        }
        else
        {
            if(number != "")
            {
                tokens.push_back(number);
                lastNumber = number;
                number = "";
            } else if(parameterName != "")
            {
                string value = to_string(getVertexParameterValue(parameterName, params, v));
                tokens.push_back(value);
                lastNumber = value;
                parameterName = "";
                readingFromExpr = false;
            }
            if(isOperator(m))
            {
                if(lastNumber == "") {
                    tokens.push_back("0");
                }
                while(numberStack.size() > 0)
                {
                    string last = numberStack.top();
                    if(getPriority(last[0]) >= getPriority(m))
                    {
                        tokens.push_back(last);
                        numberStack.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                string newString = "";
                newString.push_back(m);
                numberStack.push(newString);
            }
            else if(m == '(')
            {
                string newString = "";
                newString.push_back(m);
                numberStack.push(newString);
                lastNumber = "";
            }
            else if(m == ')')
            {
                while(numberStack.size() > 0) {
                    string last = numberStack.top();
                    numberStack.pop();
                    if(last != "(")
                    {
                        tokens.push_back(last);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    if(number != "")
    {
        tokens.push_back(number);
    } else if(parameterName != "")
    {
        string value = to_string(getVertexParameterValue(parameterName, params, v));
        tokens.push_back(value);
        lastNumber = value;
    }
    while(!numberStack.empty())
    {
        tokens.push_back(numberStack.top());
        numberStack.pop();
    }
    stack<float> operands;
    vector<string>::iterator tIt;
    for(tIt = tokens.begin(); tIt < tokens.end(); tIt++)
    {
        string token = (*tIt);
        if(token.size() == 1 && isOperator(token[0]))
        {
            float y = operands.top();
            operands.pop();
            float x = operands.top();
            operands.pop();
            operands.push(getVal(x, y, token[0]));
        }
        else
        {
            operands.push(stof(token));
        }
    }
    if(operands.empty())
    {
        return 0.0f;
    }
    return operands.top();
}

bool isOperator(char token)
{
    return token == '+' || token == '-' || token == '*' || token == '/';
}

float getParameterValue(string name, unordered_map<string, Parameter> *params)
{
    unordered_map<string, Parameter>::iterator pIt;
    pIt = params -> find(name);
    if(pIt == params -> end())
    {
        std::cout<<"Warning: Parameter " + name + " has not be defined yet.";
        return 0.0f;
    }
    return (pIt -> second).getValue();
}

float getMeshParameterValue(string name, unordered_map<string, Parameter> *params, Mesh *mesh)
{
    unordered_map<string, Parameter>::iterator pIt;
    pIt = params -> find(name);
    if(pIt == params -> end())
    {
        std::cout<<"Warning: Parameter " + name + " has not be defined yet.";
        return 0.0f;
    }
    mesh -> addParam(&(pIt -> second));
    return (pIt -> second).getValue();
}

float getTransformationParameterValue(string name,
                                      unordered_map<string, Parameter> *params,
                                      Transformation *t)
{
    unordered_map<string, Parameter>::iterator pIt;
    pIt = params -> find(name);
    if(pIt == params -> end())
    {
        std::cout<<"Warning: Parameter " + name + " has not be defined yet.";
        return 0.0f;
    }
    t -> addParam(&(pIt -> second));
    return (pIt -> second).getValue();
}

float getVertexParameterValue(string name,
                              unordered_map<string, Parameter> *params,
                              Vertex *v)
{
    unordered_map<string, Parameter>::iterator pIt;
    pIt = params -> find(name);
    if(pIt == params -> end())
    {
        std::cout<<"Warning: Parameter " + name + " has not be defined yet.";
        return 0.0f;
    }
    v -> addParam(&(pIt -> second));
    return (pIt -> second).getValue();
}

float getVal(float x, float y, char oper)
{
    switch(oper)
    {
        case '+':
            return x + y;
        case '-':
            return x - y;
        case '*':
            return x * y;
        case '/':
            return x / y;
        default:
            cout<<"Warning: operator is not defined."<<endl;
            return 0.0f;
    }
}

int getPriority(char oper)
{
    switch(oper)
    {
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
        default:
            return 0;
    }
}

vec3 getXYZ(string input)
{
    //cout<<input<<endl;
    string number = "";
    int i = 0;
    float x;
    float y;
    float z;
    for(char& c : input)
    {
        if((c >= '0' &&  c <= '9') || c == '.')
        {
            number.push_back(c);
        }
        else
        {
            if(number != "")
            {
                switch(i)
                {
                case 0:
                    x = stof(number);
                    break;
                case 1:
                    y = stof(number);
                    break;
                case 2:
                    z = stof(number);
                    break;
                }
                number = "";
                i++;
            }
        }
    }
    if(number != "")
    {
        switch(i)
        {
        case 0:
            x = stoi(number);
            break;
        case 1:
            y = stof(number);
            break;
        case 2:
            z = stof(number);
            break;
        }
    }
    return vec3(x, y, z);
}

float getOneValue(string input)
{
    //cout<<input<<endl;
    string number = "";
    for(char& c : input)
    {
        if((c >= '0' &&  c <= '9') || c == '.' || c == '-' || c == '+')
        {
            number.push_back(c);
        }
        else
        {
            if(number != "")
            {
                return stoi(number);
            }
        }
    }
    if(number != "")
    {
        return stof(number);
    }
    return 0.0f;
}

vec4 getXYZW(string input)
{
    cout<<input;
    string number = "";
    int i = 0;
    float x;
    float y;
    float z;
    float w;
    for(char& c : input)
    {
        if((c >= '0' &&  c <= '9') || c == '.' || c == '-' || c == '+')
        {
            number.push_back(c);
        }
        else
        {
            if(number != "")
            {
                switch(i)
                {
                case 0:
                    x = stoi(number);
                    break;
                case 1:
                    y = stof(number);
                    break;
                case 2:
                    z = stof(number);
                    break;
                case 3:
                    w = stof(number);
                }
                number = "";
                i++;
            }
        }
    }
    if(number != "")
    {
        switch(i)
        {
        case 0:
            x = stoi(number);
            break;
        case 1:
            y = stof(number);
            break;
        case 2:
            z = stof(number);
            break;
        case 3:
            w = stof(number);
        }
    }
    return vec4(x, y, z, w);
}


QColor evaluate_color_expression(string input)
{
    float r, b, g;
    string number = "";
    int i = 0;
    for(char& c : input)
    {
        if(((c >= '0' &&  c <= '9') || c == '.' || c == '-' || c == '+'))
        {
            number.push_back(c);
        }
        else
        {
            if(number != "")
            {
                switch(i)
                {
                case 0:
                    r = stof(number);
                    break;
                case 1:
                    g = stof(number);
                    break;
                case 2:
                    b = stof(number);
                    break;
                }
                number = "";
                i++;
            }
        }
    }
    if(number != "")
    {
        switch(i)
        {
        case 0:
            r = stof(number);
            break;
        case 1:
            g = stof(number);
            break;
        case 2:
            b = stof(number);
            break;
        }
        number = "";
        i++;
    }
    //cout<<r<<" "<<g<<" "<<b<<endl;
    return QColor(255*r, 255*g, 255*b);
}
