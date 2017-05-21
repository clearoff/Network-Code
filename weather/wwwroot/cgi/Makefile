all:cgi_add weather_show
cgi_add :cgi_add.c 
	gcc -o $@ $^
weather_show:weather_show.cpp
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -rf cgi_add 
	rm -rf weather_show

