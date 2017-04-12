CC=g++
LIB=-L /usr/lib/mysql
INCLUDE=-I /usr/include/mysql
lib=-lmysqlclient

cgi_sql:cgi_sql.cpp sql_api.cpp
	$(CC) -o $@ $^ $(LIB) $(INCLUDE) $(lib) -g
.PHONY:clean
clean:
	rm -rf cgi_sql
