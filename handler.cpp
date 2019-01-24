#include <iostream>
#include <string>
#include <iomanip>
#include <unistd.h>

#include "handler.h"
#include "rmtool.h"

#define RESET_NUM  0.95 //当内存使用量超过此阈值(百分比)时，重启geth
#define EMAIL_NUM  0.9  //邮件报警百分比

#define GET_MEM      "cat /proc/meminfo"
#define RESTART_GETH "supervisorctl restart geth"

#define SLEEP_TIME 60 * 30

using namespace std;

Handler::Handler():
    totlal_value(""),
    free_value("")
{

}

/*
 * 获取系统的内存总量及空余量
*/
int Handler::getMemery()
{
    string rst;
    if(0 > RmTool::getShellResult(GET_MEM, rst, false))
    {
        return -1;
    }

    if(0 > RmTool::getKeyValue(rst, "MemTotal", this->totlal_value))
    {
        return -1;
    }
//    cout << totlal_value << endl;

    if(0 > RmTool::getKeyValue(rst, "MemFree", this->free_value))
    {
        return -1;
    }
//    cout << free_value << endl;

    return 0;
}

int Handler::do_restart()
{
    double total = stod(this->totlal_value);
    double free = stod(this->free_value);

//    cout << total << endl;
//    cout << free << endl;
    double percentage = (total - free) / total;

    cout<<setprecision(2)<< percentage << endl;

    string tmp;

    if(RESET_NUM < (total - free) / total)
    {
        RmTool::getShellResult(RESTART_GETH, tmp, false);
    }
    else if (EMAIL_NUM < (total - free) / total)
    {
        // todo
    }
    else
    {
        //nothing to do
    }

    return 0;
}

int Handler::start()
{
    while (1)
    {
        if(0 > getMemery())
        {
            return -1;
        }

        do_restart();

        sleep(SLEEP_TIME);
    }

    return 0;
}
