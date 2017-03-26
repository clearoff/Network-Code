#########################################################################
# File Name: caclu.sh
# Author: LZH
# mail:597995302@qq.com
# Created Time: Sun 26 Mar 2017 11:40:52 AM PDT
#########################################################################
#!/bin/bash

if [ $# -lt 3 ];then
	echo 'input error'
	exit 1
else
	echo 'input success'
fi

min=$1
max=$1
sum=0
for((i=1;i<=$#;i++))
do
	#echo $i
	if [ ${min} -lt $i ];then
		let min=$i
	fi

	if [ ${max} -gt $i ];then
		let max=$i
	fi
	let sum+=$i
done

echo "ibase=10; scale=2; $sum/$#" | bc
echo 'mim:'$min
echo 'max:'$max
