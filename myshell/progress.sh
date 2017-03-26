#########################################################################
# File Name: progress.sh
# Author: LZH
# mail:597995302@qq.com
# Created Time: Sun 26 Mar 2017 11:26:03 AM PDT
#########################################################################
#!/bin/bash
#shebang

buf=''
table=('-' '|' '/' '-' '\\')
for((i=0;i<=100;i++))
do
	printf "[%-100s][%d%%][%c]\r" "${buf}" "$i" "${table[i%5]}"
	#echo ${buf}
	buf=${buf}#
	sleep 0.1
done
echo '\n'

