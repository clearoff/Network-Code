#!/bin/bash

ROOT_PATH=$(pwd)
#echo $ROOT_PATH
SRC=$(ls *.c | tr '\n' ' ') 
echo $SRC
OBJ=$(echo ${SRC} | sed 's/\.c/\.o/g')
echo $OBJ
SERVER_NAME=httpd
CC=gcc
LOG=log
LIB='-lpthread'
PID=${ROOT_PATH}/httpd.pid
CTL="http_ctl.sh"
CONF="conf"
touch ${LOG}


#cgi
CGISRC=$(basename ./wwwroot/cgi/*.c | tr '\n' ' ')
#echo ${CGISRC}
CGI=$(echo ${CGISRC} | sed 's/\.c/ /g')
WWW_PATH=${ROOT_PATH}"/wwwroot"

#output project
OUT=output

touch ${LOG}
cat << EOF > ${WWW_PATH}/cgi/Makefile
${CGI}:${CGISRC}
	${CC} -o \$@ \$^
.PHONY:clean
clean:
	rm -rf ${CGI}

EOF

cat << EOF >Makefile
.PHONY:all
all: ${SERVER_NAME}
${SERVER_NAME}:${OBJ}
	${CC} -o \$@ \$^ ${LIB}
%.o:%.c
	${CC} -c \$<
.PHONY:clean
clean:
	rm -rf ${OBJ} ${SERVER_NAME} ${LOG} ${OUT} ${PID}
	cd ${WWW_PATH}/cgi;make clean; cd -
	cd ${ROOT_PATH}/testsql make clean; cd -

.PHONY:output
output:all
	touch log
	rm -rf ${OUT}
	mkdir ${OUT} 
	cp ${CONF} ${OUT}/ -R 
	cp ${CTL} ${OUT}/
	make
	cd ${WWW_PATH}/cgi/ ;make;cd -
	mv ${SERVER_NAME} ${OUT}/
	mv ${LOG} ${OUT}/
	cp ${WWW_PATH} ${OUT}/ -R
EOF

