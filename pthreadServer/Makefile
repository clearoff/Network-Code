.PHONY:all
all:client tserver
client:client.c
	gcc -o $@ $^
tserver:tserver.c
	gcc -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f client tserver
