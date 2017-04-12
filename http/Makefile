.PHONY:all
all: httpd
httpd:httpd.o main.o
	gcc -o $@ $^ -lpthread
%.o:%.c
	gcc -c $<
.PHONY:clean
clean:
	rm -rf httpd.o main.o httpd log output /home/lzh/code/http/httpd.pid
	cd /home/lzh/code/http/wwwroot/cgi;make clean; cd -
.PHONY:output
output:all
	rm -rf output
	mkdir output 
	cp conf output/ -R 
	cp http_ctl.sh output/
	make
	cd /home/lzh/code/http/wwwroot/cgi/ ;make;cd -
	mv httpd output/
	mv log output/
	cd /home/lzh/code/http/testsql;make;cp cgi_sql /home/lzh/code/http/wwwroot/cgi/;cd -
	cp /home/lzh/code/http/wwwroot output/ -R
