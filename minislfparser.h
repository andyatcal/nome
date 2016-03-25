#ifndef MINISLFPARSER_H
#define MINISLFPARSER_H

#include <vector>
#include "parameterbank.h"
#include "funnel.h"
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
    void makeWithMiniSLF(vector<ParameterBank> &banks, Group &group, string inputSIF);
};

#endif // MINISLFPARSER_H
