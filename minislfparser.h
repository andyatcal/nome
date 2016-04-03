#ifndef MINISLFPARSER_H
#define MINISLFPARSER_H

#include <vector>
#include "parameterbank.h"
#include "mesh.h"
#include "group.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
class MiniSlfParser
{
public:
    MiniSlfParser();
    void makeWithMiniSLF(vector<ParameterBank> &banks,
                         unordered_map<string, Parameter> &params,
                         Group &group, string inputSIF);
    void appendWithASLF(vector<ParameterBank> &banks,
                         unordered_map<string, Parameter> &params,
                         Group &group, string inputSIF);

};

#endif // MINISLFPARSER_H
