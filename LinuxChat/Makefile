ROOT_PATH=$(shell pwd)
SERVER_PATH=$(ROOT_PATH)/server
CLIENT_PATH=$(ROOT_PATH)/client
WINDOW_PATH=$(ROOT_PATH)/window
COMM_PATH=$(ROOT_PATH)/comm
LOG_PATH=$(ROOT_PATH)/log
POOL_PATH=$(ROOT_PATH)/pool
#REDIS_PATH=$(ROOT_PATH)/redis-lib
REDIS_PATH=$(ROOT_PATH)/redis-lib
JSON_PATH=$(ROOT_PATH)/Json
SERVER_BIN=udpServer
CLIENT_BIN=udpClient
SERVER_SRC=$(shell ls $(SERVER_PATH) $(REDIS_PATH) $(POOL_PATH) $(JSON_PATH) | egrep -v '^.*h')
CLIENT_SRC=$(shell ls $(CLIENT_PATH) $(JSON_PATH) $(WINDOW_PATH)| egrep -v '^.*h')
SERVER_OBJ=$(shell echo $(SERVER_SRC) | sed 's/\.cpp/\.o/g')
CLIENT_OBJ=$(shell echo $(CLIENT_SRC) | sed 's/\.cpp/\.o/g')
CC=g++


.PHONY:debug
debug:
	@echo $(POOL_PATH)
	@echo $(SERVER_SRC)
	@echo $(SERVER_OBJ)
	@echo $(CLIENT_SRC)
	@echo $(CLIENT_OBJ)

.PHONY:all
all:$(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN):$(SERVER_OBJ)  
		${CC} -o $@ $^ -lhiredis -lpthread -ljson
%.o:$(SERVER_PATH)/%.cpp
		${CC} -c $<
%.o:$(POOL_PATH)/%.cpp 
		$(CC) -c $<
%.o:$(JSON_PATH)/%.cpp 
		$(CC) -c $< 
%.o:$(REDIS_PATH)/%.cpp
		$(CC) -c $<

$(CLIENT_BIN):$(CLIENT_OBJ)
		$(CC) -o $@ $^ -lpthread -ljson -lncurses 
%.o:$(CLIENT_PATH)/%.cpp 
		$(CC) -c $<
%.o:$(JSON_PATH)/%.cpp 
		$(CC) -c $<
%.o:$(WINDOW_PATH)/%.cpp 
		$(CC) -c $<


.PHONY:clean
clean:
	rm -rf *.o $(SERVER_BIN) $(CLIENT_BIN)
	rm -rf output

.PHONY:output
output:
	mkdir output
	make all
	mv ${SERVER_BIN} ${ROOT_PATH}/output
	mv ${CLIENT_BIN} ${ROOT_PATH}/output

