#ifndef MINISLFPARSER_H
#define MINISLFPARSER_H

#include <vector>
#include "parameterbank.h"
#include "makeMesh.h"
#include "group.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
class MiniSlfParser
{
public:
    MiniSlfParser();
    void makeWithMiniSLF(vector<ParameterBank> &banks, Group &group, string inputSIF);
};

#endif // MINISLFPARSER_H
