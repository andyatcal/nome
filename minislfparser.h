#ifndef MINISLFPARSER_H
#define MINISLFPARSER_H

#include <vector>
#include "parameterbank.h"
#include "group.h"
class MiniSlfParser
{
public:
    MiniSlfParser();
    vector<ParameterBank> banks;
    vector<Group> groups;
    void makeWithMiniSIF(vector<ParameterBank> &banks, vector<Group> &groups, string inputSIF);
};

#endif // MINISLFPARSER_H
