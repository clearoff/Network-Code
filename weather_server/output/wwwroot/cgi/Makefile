ROOT_PATH=$(shell pwd)
REDIS_API=$(ROOT_PATH)/redis-lib
REGISTER=register

all:register cgi_add weather_show
cgi_add :cgi_add.c 
	gcc -o $@ $^
weather_show:weather_show.cpp
	g++ -o $@ $^
register:register.o redis_api.o
	g++ -o $@ $^ -lhiredis 
register.o:register.cpp
	g++ -c $<
redis_api.o:redis-lib/redis_api.cpp
	g++ -c $<
.PHONY:clean
clean:
	rm -rf cgi_add 
	rm -rf weather_show
	rm -rf register
	rm -rf register.o
	rm -rf redis_api.o

