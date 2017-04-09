#!/bin/bash

ROOT_PATH=`pwd`
BIN=${ROOT_PATH}/httpd
CONF=${ROOT_PATH}/conf/httpd.conf
PID=${ROOT_PATH}/httpd.pid
proc=`basename $0`

function useage(){
	printf "%s [start (-s) |stop (-t) | restart(-rt)]\n" "$proc"
}

function http_start(){
	echo "Debug 2\n"
	[[ -f $PID ]] && {
		echo "Debug 3\n"
		echo $BIN
		[[ ! -f "httpd" ]] && {
			echo "httpd file is not exit...\n"
			return
		}
		printf "httpd is exit,pid is $(cat $pid)\n"
		return 
	}	
	ip=$(grep -E '^IP' "./conf/httpd.conf" | awk -F: '{print $2}')
	port=$(grep -E '^PORT' "./conf/httpd.conf" | awk -F: '{print $2}')
	echo $ip
	echo $port
	${BIN} ${ip} ${port}
	pidof $(basename $BIN) > $PID
	printf "Start done,pid is : $(cat ${PID})...\n"
}

function http_stop(){
	echo "Debug 1\n"
	[[ ! -f $PID ]] && {
		printf "httpd is not exist!\n"
		return 
	}	
	pid=$(cat $PID)
	kill -9 $pid
	rm -f $PID
	printf "stop done ...\n"
}

function http_restart(){
	http_stop
	http_start
}

#echo $#
[ $# -ne 1 ]&&{
	useage
	exit 1
}

case $1 in
	start | -s )
		echo "http will start\n"
		http_start
	;;
	stop | -t )
		http_stop
	;;
	restart | -rt)
		http_restart
	;;
	* )
		useage
		exit 2
	;;
esac

