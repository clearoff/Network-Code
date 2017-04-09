#ifndef _HTTPD_H__
#define _HTTPD_H__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <syslog.h>



#define HOME "wwwroot"
#define NORMAL  0
#define FATAL   1
#define WARNING 2
#define BUF_SIZE 1024

int StartUp(const char *_ip,int port);
int HandleSock(int sock);
void PrintLog(const char* msg,int level);
#endif //_HTTP_H__
