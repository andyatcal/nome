#include "funnel.h"

Funnel::Funnel()
{
    n = 0;
    ro = 1.0f;
    ratio = 0.0f;
    h = 1.0f;
    n_expr = "";
    ro_expr = "";
    ratio_expr = "";
    h_expr = "";
}

void Funnel::setParameterValues(string input)
{
    string nextExpression = "";
    bool expressionMode = false;
    string number = "";
    int i = 0;
    for(char& c : input)
    {
        if(c == '{')
        {
            expressionMode = true;
        }
        else if(c == '}')
        {
            cout<<nextExpression<<endl;
            expressionMode = false;
            switch(i)
            {
            case 0:
                n_expr = nextExpression.substr(5);
                n = int(evaluate_expression(n_expr, params) + 0.5);
                break;
            case 1:
                ro_expr = nextExpression.substr(5);
                ro = evaluate_expression(ro_expr, params);
                break;
            case 2:
                ratio_expr = nextExpression.substr(5);
                ratio = evaluate_expression(ratio_expr, params);
                break;
            case 3:
                h_expr = nextExpression.substr(5);
                h = evaluate_expression(h_expr, params);
                break;
            }
            nextExpression = "";
            i++;
        }
        else if(expressionMode)
        {
            nextExpression.push_back(c);
        }
        else if(!expressionMode && ((c >= '0' &&  c <= '9') || c == '.'))
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
                    n = stoi(number);
                    break;
                case 1:
                    ro = stof(number);
                    break;
                case 2:
                    ratio = stof(number);
                    break;
                case 3:
                    h = stof(number);
                    break;
                }
                number = "";
                i++;
            }
        }
    }
}

void Funnel::updateFunnel()
{
    if(n_expr != "")
    {
        int new_n = evaluate_expression(n_expr, params);
        if(new_n != n)
        {
            n = new_n;
            updateFunnel_n();
        }
    }
    if(ro_expr != "")
    {
        float new_ro = evaluate_expression(ro_expr, params);
        if(new_ro != ro)
        {
            ro = new_ro;
            updateFunnel_ro();
        }
    }
    if(ratio_expr != "")
    {
        float new_ratio = evaluate_expression(ratio_expr, params);
        if(new_ratio != ratio)
        {
            ratio = new_ratio;
            updateFunnel_ratio_or_h();
        }
    }
    if(h_expr != "")
    {
        float new_h = evaluate_expression(h_expr, params);
        if(new_h != h)
        {
            h = new_h;
            updateFunnel_ratio_or_h();
        }
    }
}

void Funnel::updateFunnel_n()
{
    makeFunnel();
}

void Funnel::updateFunnel_ro()
{
    for(int i = 0; i < n; i++)
    {
        Vertex * newVertex = vertList[i];
        float currAngle = 2.0 * i / n * PI;
        newVertex -> position = vec3(ro * glm::cos(currAngle), ro * glm::sin(currAngle), 0);
    }
    updateFunnel_ratio_or_h();
}

void Funnel::updateFunnel_ratio_or_h()
{
    float ri = ro * (1 + ratio);
    for(int i = 0; i < n; i++)
    {
        Vertex * newVertex = vertList[i + n];
        float currAngle = 2.0 * i / n * PI;
        newVertex -> position = vec3(ri * glm::cos(currAngle), ri * glm::sin(currAngle), h);
    }
}

void Funnel::makeFunnel()
{
    if(n < 3) {
        return;
    }
    vertList.clear();
    edgeTable.clear();
    faceList.clear();
    vector<Vertex*> baseCircle;
    vector<Vertex*> highCircle;
    for(int i = 0; i < n; i++)
    {
        Vertex * newVertex = new Vertex;
        newVertex->ID = i;
        float currAngle = 2.0 * i / n * PI;
        newVertex -> position = vec3(ro * glm::cos(currAngle), ro * glm::sin(currAngle), 0);
        baseCircle.push_back(newVertex);
        addVertex(newVertex);
    }
    float ri = ro * (1 + ratio);
    for(int i = 0; i < n; i++)
    {
        Vertex * newVertex = new Vertex;
        newVertex->ID = i + n;
        float currAngle = 2.0 * i / n * PI;
        newVertex -> position = vec3(ri * glm::cos(currAngle), ri * glm::sin(currAngle), h);
        highCircle.push_back(newVertex);
        addVertex(newVertex);
    }
    for(int i = 0; i < n - 1 ; i++)
    {
        addQuadFace(baseCircle[i], baseCircle[i + 1], highCircle[i + 1], highCircle[i]);
    }
    addQuadFace(baseCircle[n - 1], baseCircle[0], highCircle[0], highCircle[n - 1]);
    buildBoundary();
}
