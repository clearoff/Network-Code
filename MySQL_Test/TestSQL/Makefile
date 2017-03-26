testsql:testsql.c
	gcc -o $@ $^ -I /usr/include/mysql -L/usr/lib/mysql -lmysqlclient

.PHONY:clean
clean:
	rm -f testsql
