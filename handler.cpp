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
#define SEND_EMAIL   "bash ./sendEmail.sh "

#define MEM_TOTAL "MemTotal"
#define MEM_FREE  "MemAvailable"

#define SLEEP_TIME 60 * 30
#define ZHH_DEBUG 0

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
    cout << "total memory: " << totlal_value << " kb" <<  endl;

    if(0 > RmTool::getKeyValue(rst, MEM_FREE, this->free_value))
    {
        return -1;
    }
    cout << "free  memory: " <<free_value << " kb" << endl;

    return 0;
}

int do_reset_geth(string content)
{
    string tmp;
    string src = SEND_EMAIL + content;

    RmTool::getShellResult(src, tmp, false);
#if !ZHH_DEBUG
    RmTool::getShellResult(RESTART_GETH, tmp, false);
#endif
    return 0;
}

int Handler::do_distribution()
{
    double total = stod(this->totlal_value);
    double free = stod(this->free_value);

    double percentage = (total - free) / total;

    cout << "当前内存使用率为：" << setprecision(4) << percentage << endl;

    if(RESET_NUM < (total - free) / total)
    {
        do_reset_geth(to_string(free));
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

        do_distribution();

        sleep(SLEEP_TIME);
    }

    return 0;
}
