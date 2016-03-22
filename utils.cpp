#include "utils.h"

float evaluate_expression(string expr, unordered_map<string, Parameter> *params)
{
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
                string value = to_string(getParameterValue(parameterName, params));
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
        if(isOperator(token[0]))
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
