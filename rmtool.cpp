#include <glog/logging.h>
#include <stdlib.h>
#include <string.h>

#include "rmtool.h"

using namespace  std;

#define HTTP_PORT 8888
#define BUF_SIZE  1024

RmTool::RmTool()
{

}

/*
 * 接口功能：执行脚本命令，得到结果
 * cmd: 运行脚本的命令
 * src: 输出参数，脚本的输出
 * delete_tail: 是否去除返回值末尾的空格字符，默认为否
 * 返回值: 0为成功，负数为失败
*/
int RmTool::getShellResult(const string cmd, string& src, bool delete_tail)
{
    FILE * fp;
    char buffer[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);
    fp = popen(cmd.c_str(), "r") ;

    try
    {
        while (fgets(buffer, BUF_SIZE, fp) != NULL)
        {
            src += buffer;
        }

        //去掉末尾的空格、回车等多余字符
        if(delete_tail)
        {
            ostringstream os;

            for(unsigned int i = 0; i < src.size(); i++)
            {
                if(src[i] >= '0' && src[i] <= 'f')
                {
                    os << src[i];
                }
            }
            src = os.str();
        }

        pclose(fp) ;
    }
    catch(std::out_of_range)//异常
    {
        pclose(fp) ;
        return -1;
    }

    return 0;
}

/*
 * 接口功能：从字符串中获取第一个key所对应的值,以字符串的形式返回
 * srcstr:源字符串，一定要为json字符串格式
 * key : 查询的字段名称
 * value: 回参，返回的结果
 * 返回值: 0为获取成功，负数为获取失败
 * 说明：
 *
 * 假设源符串为
 *   {
    "txid": "4a9ddd37dd18a705ca2b8a1fd3e85fa9a839a96174e943c5ade82298eacd929e",
    "vout": 2,
    "address": "1MyNPSmykUj7NH2N1A922UFUXxPaicAdJE",
    "account": "",
    "scriptPubKey": "76a914e60ba998604f9d8dfcd32cbf0a32a5b94a2a6d0e88ac",
    "amount": 0.00000546,
    "confirmations": 63,
    "spendable": true,
    "solvable": true
  }
*/
int RmTool::getKeyValue(const string srcstr, const string key, string& value)
{
    if(srcstr.empty() || key.empty())
    {
        LOG(ERROR) << "The parameter is empty!";
        return -1;
    }

    try
    {
        int start = srcstr.find(key);
        if(-1 == start)
        {
            LOG(ERROR) << "Can't find [" << key << "] in the src stirng [" << srcstr << "]";
            return -2;
        }

        int end = srcstr.find(":", start);

        /*
         * 找到非空格
         *  MemTotal:        8197816 kB
            MemFree:         1290880 kB
            MemAvailable:    5834304 kB
            Buffers:          312764 kB
            Cached:          4243172 kB
            SwapCached:            0 kB
            Active:          4365724 kB
            Inactive:        2214484 kB
            Active(anon):    2024452 kB
            Inactive(anon):      232 kB
        */
        for(start = end + 1; srcstr[start] == ' ';start++);

        // 再获取两个空格之间的数值，单位不用管，就让它默认是KB
        end   = srcstr.find(" ", start);
        value = srcstr.substr(start, end - start);

    }
    catch(std::out_of_range)
    {
        return -1;
    }

    return 0;
}
