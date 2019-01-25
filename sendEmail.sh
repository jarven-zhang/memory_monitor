#########################################################################
# File Name: sendEmail.sh
# Author: jarven
# mail: hua_0602@163.com
# Created Time: 2019年01月25日 星期五 16时31分13秒
#########################################################################
#!/bin/bash

cur_data=`date +%Y-%m-%d,%H:%M:%S`
content=${cur_data}"\n\n\n当前内存剩余:""$1""kb"

echo -e ${content} | mail -s "ETH节点服务器内存不足，将要重启geth客户端！" jarven@rockminer.com
echo -e ${content} | mail -s "ETH节点服务器内存不足，将要重启geth客户端！" admin@rockminer.com

