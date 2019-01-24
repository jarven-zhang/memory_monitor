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

#define MEM_TOTAL "MemTotal"
#define MEM_FREE  "MemAvailable"

#define SLEEP_TIME 60 * 30
#define ZHH_DEBUG 1

using namespace std;

Handler::Handler():
    totlal_value(""),
    free_value("")
{

}

/*
 * 获取系统的内存总量及空余量
*/
int Handler::getMemory()
{
    string rst;
    if(0 > RmTool::getShellResult(GET_MEM, rst, false))
    {
        return -1;
    }

    if(0 > RmTool::getKeyValue(rst, MEM_TOTAL, this->totlal_value))
    {
        return -1;
    }
#if ZHH_DEBUG
    cout << "total memory: " << totlal_value << " kb" <<  endl;
#endif

    if(0 > RmTool::getKeyValue(rst, MEM_FREE, this->free_value))
    {
        return -1;
    }
#if ZHH_DEBUG
    cout << "free  memory: " <<free_value << " kb" << endl;
#endif

    return 0;
}

int Handler::do_restart()
{
    double total = stod(this->totlal_value);
    double free = stod(this->free_value);

    double percentage = (total - free) / total;

    cout<<setprecision(2)<< percentage << endl;

    string tmp;

    if(RESET_NUM < (total - free) / total)
    {
#if ZHH_DEBUG
#else
        RmTool::getShellResult(RESTART_GETH, tmp, false);
#endif
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
        if(0 > getMemory())
        {
            return -1;
        }

        do_restart();

        sleep(SLEEP_TIME);
    }

    return 0;
}
