CC=g++
LIB=-L /usr/lib/mysql
INCLUDE=-I /usr/include/mysql
lib=-lmysqlclient

.PHONY:all
all:cgi_sql cgi_show
cgi_sql:cgi_sql.cpp sql_api.cpp
	$(CC) -o $@ $^ $(LIB) $(INCLUDE) $(lib) -g
cgi_show:cgi_show.cpp sql_api.cpp
	$(CC) -o $@ $^ $(LIB) $(INCLUDE) $(lib) -g
.PHONY:clean
clean:
	rm -rf cgi_sql	cgi_show
