.PHONY:all
all: httpd
httpd:httpd.o main.o
	gcc -o $@ $^ -lpthread
%.o:%.c
	gcc -c $<
.PHONY:clean
clean:
	rm -rf httpd.o main.o httpd log output /home/lzh/Desktop/code/http/httpd.pid
	cd /home/lzh/Desktop/code/http/wwwroot/cgi;make clean; cd -
	cd /home/lzh/Desktop/code/http/testsql make clean; cd -

.PHONY:output
output:all
	touch log
	rm -rf output
	mkdir output 
	cp conf output/ -R 
	cp http_ctl.sh output/
	make
	cd /home/lzh/Desktop/code/http/wwwroot/cgi/ ;make;cd -
	mv httpd output/
	mv log output/
	cp /home/lzh/Desktop/code/http/wwwroot output/ -R
