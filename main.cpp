#include <iostream>
#include "rmtool.h"

using namespace std;

int foo()
{
    string exe = "cat /proc/meminfo";
    string rst;
    RmTool::getShellResult(exe, rst, false);

    string totlal_value("");
    RmTool::getKeyValue(rst, "MemTotal", totlal_value);

    cout << totlal_value << endl;

    string used_value("");
    RmTool::getKeyValue(rst, "MemFree", used_value);

    cout << used_value << endl;
    return 0;
}

int main()
{
//    cout << "Hello World!" << endl;
    foo();
    return 0;
}
