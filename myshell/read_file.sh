#!/bin/bash

i=0
arr=('' '' '')
echo 'Instead front..'
while read line
do
	echo $line
	arr[i]="$line"
	echo ${arr[i]}
	let i++
done < file 

echo ${arr[0]}
echo ${arr[1]}
echo ${arr[2]}

echo "After Instead..."
echo ${#arr[*]}
"">FILE
for((i=0;i<${#arr[*]};i++))
do
	echo "$(echo ${arr[i]} | tr '[:upper:]' '[:lower:]')" >>FILE
done
#echo ${arr[0]}

