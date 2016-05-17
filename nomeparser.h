#ifndef NOMEPARSER_H
#define NOMEPARSER_H

#include <vector>
#include "parameterbank.h"
#include "slideglwidget.h"
#include "mesh.h"
#include "polyline.h"
#include "group.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
class NomeParser
{
public:
    NomeParser();
    void makeWithNome(vector<ParameterBank> &banks,
                      unordered_map<string, Parameter> &params,
                      Group &group,
                      string inputSIF,
                      vector<string> &colorlines,
                      vector<string> &banklines,
                      vector<string> &geometrylines,
                      vector<string> &postProcessingLines);

    void postProcessingWithNome(unordered_map<string, Parameter> &params,
                                vector<string> &postProcessingLines,
                                SlideGLWidget * canvas,
                                Group &group);

    void appendWithANOM(unordered_map<string, Parameter> &params,
                        Group &group,
                        SlideGLWidget * canvas,
                        string inputSIF);

};

#endif // MINISLFPARSER_H
