#ifndef RMTOOL_H
#define RMTOOL_H
#include <iostream>
using namespace std;

class RmTool
{
public:
    RmTool();

    static int getShellResult(const string cmd, string& src, bool delete_tail);

    static int getKeyValue(const string srcstr, const string key, string& value);
};

#endif // RMTOOL_H
