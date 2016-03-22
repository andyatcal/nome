#include "utils.h"

float evaluate_expression(string expr, unordered_map<string, Parameter> *params)
{
    return 0.0;

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
