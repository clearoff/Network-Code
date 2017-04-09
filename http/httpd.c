#include "httpd.h"
static size_t LogCount=0;

static void clean_hander(int);
static int echo_code(int sock)
{
	char buf[1024];
	const char* path="./wwwroot/404.html";
	printf("path:%s\n",path);
	struct stat st;
	stat(path,&st);
	printf("%d\n",st.st_size);
	clean_hander(sock);
	int fd=open(path,O_RDONLY);
	if( fd < 0){
		//printLog("file is can't open",WARNING);
		return -2;
	}
	//strcpy(buf,"HTTP/1.0 404 NOT FOUND\r\n\r\n");
	sprintf(buf,"HTTP/1.0 200 OK\r\n\r\n");
	write(sock,buf,strlen(buf));	
	if(sendfile(sock,fd,NULL,st.st_size) < 0){
		//printLog();
		//echo_errno();

	}
	close(fd);
	return 0;
//	printf("%s\n",buf);
//	send(sock,buf,strlen(buf),0);
//	sprintf(buf,"Content-Type: text/html\r\n");
//	send(sock,buf,strlen(buf),0);
//	send(sock,"\r\n",strlen("\r\n"),0);
//	sprintf(buf,"<html><title>%s</title>\r\n",code);
//	send(sock,buf,strlen(buf),0);
//	sprintf(buf,"<body><p>%s\r\n",code);
//	send(sock,buf,strlen(buf),0);
//	sprintf(buf,"</body><br/>\r\n");
//	send(sock,buf,strlen(buf),0);
//	sprintf(buf,"</html><br/>\r\n");
//	send(sock,buf,strlen(buf),0);

}
int StartUp(const char *_ip,int port)
{
	assert(_ip);
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0){
		PrintLog("create sock error",FATAL);	
		closelog();
		exit(1);
	}
	int flag=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr=inet_addr(_ip);
	if(bind(sock,(struct sockaddr*)&addr,sizeof(addr)) < 0){
		echo_code(sock);
		exit(2);
	}
	if(listen(sock,5) < 0){
		echo_code(sock);
		exit(3);
	}
	PrintLog("Sock listend Success",NORMAL);
	printf("sock:%d\n",sock);
	return sock;
}

//void PrintLog(const char* msg,int Level)
//{
//	assert(msg);
//	const char* LevelList[]={
//		"NORMAL",
//		"FATAL",
//		"WARNING"
//	};
//	char buf[256]={0};
//	sprintf(buf,"%s%s\n",msg,LevelList[Level]);
//	if(LevelList[Level]=="NORMAL"){
//		syslog(LOG_INFO,buf,strlen(buf));
//	}
//	if(LevelList[Level]=="WARNING"){
//		syslog(LOG_WARNING,buf,strlen(buf));
//	}
//	if(LevelList[Level]=="FATAL"){
//		syslog(LOG_ERR,buf,strlen(buf));
//	}
//}
void PrintLog(const char *msg,int Level)
{
	assert(msg);
	umask(0);
	int fd = open("./log",O_WRONLY|O_EXCL);
	if(fd < 0){
		perror("open log error");
		exit(1);
	}
	const char* LevelList[]={
		",NORMAL\n",
		",FATAL\n",
		",WARNING\n"
	};
	char buf[128];
	strcpy(buf,msg);
	int i=strlen(msg);
	strcpy(buf+i,LevelList[Level]);
	//printf("%s",buf);
	ssize_t _s=write(fd,buf,strlen(buf));
	if(_s < 0){
		perror("write log error");
		exit(2);
	}
	//printf("write bytes:%d\n",_s);
}

int get_line(int sock,char *buf,int size)
{
	int i=0;
	char c='\0';
	int _s;
	while((i < size -1)&&(c != '\n')){
		_s=recv(sock,&c,1,0);    // read a bytes from sock
		if( _s > 0){
			if(c == '\r')
			{
				_s=recv(sock,&c,1,MSG_PEEK);
				if((_s > 0)&&(c == '\n')){
					recv(sock,&c,1,0);	
				}
				else
					c='\n';
			}
			buf[i++]=c;
		}
		else if(_s == 0){
		}
		else
		{
		
		}
	}
	buf[i]='\0';
	return i;
}

static void clean_hander(int sock)
{
	int ret=0;
	char buf[BUF_SIZE]={0};

	do{
		ret=get_line(sock,buf,sizeof(buf)-1);
	}while(ret!=1 && strcmp("\n",buf));
}

static int echo_html(int sock,const char* path,ssize_t size)
{
	if(path == NULL){
		return -1;
	}
	char buf[BUF_SIZE]={0};
	clean_hander(sock);
	int fd=open(path,O_RDONLY);
	if( fd < 0){
		//printLog("file is can't open",WARNING);
		return -2;
	}
	strcpy(buf,"HTTP/1.0 200 OK\r\n\r\n");
	write(sock,buf,strlen(buf));	
	if(sendfile(sock,fd,NULL,size) < 0){
		//printLog();
		//echo_errno();

	}
	close(fd);
}

static int excu_cgi(int sock,const char*path,\
		const char* method,const char* query)
{
	char buf[BUF_SIZE]={0};
	int readnum=0;
	int clength=0;
	printf("path:%s\n",path);
	//如果http方法为GET，clean_hander
	if(strcasecmp(method,"GET")==0)
	{
		printf("cgi的get方法\n");
		clean_hander(sock);
	}
	else{//POST
		printf("cgi的post方法\n");
		readnum=get_line(sock,buf,sizeof(buf)-1);
		//printf("readnum:%\n",readnum);
		//如果没有读到空行，那么继续往下读到正文的起始位置
		while( readnum > 0 && strcmp("\n",buf))
		{
			//Content-Length
			if(strncasecmp(buf,"Content-Length: ",16)==0)
			{
				//拿到Conten-Length
				//表明消息内容的长度
				//printf("contlent-length:%s\n",buf+16);
				clength=atoi(buf+16);	
				printf("Content-Length:%d\n",clength);
			}
			readnum=get_line(sock,buf,sizeof(buf)-1);
		}//while
		printf("已经读到空行\n");
		if(clength==0)
		{
			//表示此次消息内容的长度为0
			echo_code(sock);
			printf("Conten-Length=0\n");
			return -1;
		}//fi
	}//else
		printf("already read text\n");
		memset(buf,0,sizeof(buf));
		strcpy(buf,"HTTP/1.0 200 OK\r\n\r\n");
		//发送相应请求
		//协议版本+状态码+状态码描述
		send(sock,buf,strlen(buf),0);

		//创建子进程，通过子进程运行程序处理数据
		printf("path:%s\n",path);
		int cgi_in[2];
		int cgi_out[2];
		if(pipe(cgi_in)==-1){
			//PrintLog()
			return -1;
		}//fi
		if(pipe(cgi_out)==-1){
			//PrintLog()
			return -1;
		}//fi
		int pid=fork();
		if(pid < 0){//error
			//PrintLog()
			return -1;
		}//fi
		else if(pid == 0){
			//child proc
			close(cgi_in[1]);
			close(cgi_out[0]);
			dup2(cgi_in[0],0); 
			dup2(cgi_out[1],1);
			//printf("child run\n");
			char env_method[1024]={0};
			char env_clength[1024]={0};
			char env_query[1024]={0};

			sprintf(env_method,"REQUEST_METHOD=%s",method);
			putenv(env_method);
			if(strcasecmp("GET",method)==0){
				//GET method
				//printf("QUERY_STRING=%s\n",query);
				sprintf(env_query,"QUERY_STRING=%s",query);
				putenv(env_query);
			}
			else{
				//POST method
				sprintf(env_clength,"CONTENT_LENGTH=%d",clength);
				putenv(env_clength);
			}
			//printf("path:%s\n",path);
			execl(path,path,NULL);
			//PrintLog();
			printf("执行程序失败\n");
			exit(1);     //如果execl执行失败，则终止
		}//else if
		else { //father proc
			close(cgi_in[0]);
			close(cgi_out[1]);

			int i=0;
			char ch=0;
			if(strcasecmp("POST",method)==0)
			{
				for(;i<clength;i++){
					recv(sock,&ch,1,0);
					write(cgi_in[1],&ch,1);
				}
			}
			while(read(cgi_out[0],&ch,1) > 0)
				send(sock,&ch,1,0);
			
			close(cgi_in[1]);
			close(cgi_out[0]);
			waitpid(pid,NULL,0);
		}//else
	return 0;	
}

int HandleSock(int sock)
{
	char url[255]={0};			//读取buf中的url
	char buf[BUF_SIZE+1]={0};	//读取缓冲区
	char method[255]={0};		//获取http方法的数组
	int cgi=0;
	char* query=url;
	int ReadSize=0;
	char path[BUF_SIZE]={0};

	//printf("Debug 1\n");
	if((ReadSize=get_line(sock,buf,sizeof(buf))) < 0){
		return;		
	}
	int i=0,j=0;	
	//j访问buf数组的索引
	//读取http方法，遇到空格即退出循环
	//printf("Debug 2\n");
	while(!isspace(buf[j])&&(i < sizeof(method)-1)){
		method[i] = buf[j];
		i++,j++;
	}
	method[i]='\0';
	printf("method:%s\n",method);
	i=0;
	//如果没有碰到空格，则一直向后访问
	//将协议版本过滤掉
	//printf("Debug 3\n");
	while(isspace(buf[j]) && (j<sizeof(buf)-1))
	{
		j++;
	}

	//读取buf数组中的url
	while(!isspace(buf[j]) &&\
		 (i<sizeof(url)-1) &&\
		 (j<sizeof(buf)-1)){
		url[i]=buf[j];
		i++,j++;
	}
	url[i]='\0';

	//printf("Debug URL:%s\n",url);
	//printf("method:%s\n",method);
	//strcasecmp函数比较两个字符串并忽略大小写
	//如果长度相等返回0
	//如果第一个字符串大于第二个字符串返回>0
	//else < 0
	//这里如果不是GET和POST方法那么返回-1.
	if(strcasecmp(method,"GET") &&\
	strcasecmp(method,"POST")){
		echo_code(sock);
		return -1;
	}

	//如果为POST方法，那么要考虑cgi
	if(strcasecmp(method,"POST")==0){
		cgi=1;
	}

	//如过为GET方法，读取字符串
	if(strcasecmp(method,"GET")==0){
		while(*query != '\0'){
			//这里需要判断是否通过GET方法传递了参数
			if(*query == '?'){
				cgi=1;
				*query='\0';
				query++;
				break;
			}
			query++;
		}
	}
	//将本地根目录和请求的资源路径拷贝到path中
	sprintf(path,"wwwroot%s",url);
	//这里需要判断请求的是否是目录
	if(path[strlen(path)-1]=='/'){
		strcat(path,"index.html");	
	}

	struct stat st;
	printf("url=%s\n",url);
	//通过stat系统调用判断访问文件是否存在
	if( stat(path,&st) < 0 ){
		echo_code(sock);
		clean_hander(sock);
		//echo_errno();
	}
	else{
		//文件存在
		if(S_ISDIR(st.st_mode)){
			//如果请求的资源是目录，那么需要在后面加上'/'
			printf("目录文件\n");
			strcat(path,"/index.html");
			printf("path:%s\n",path);
		}
		else if(st.st_mode & S_IXUSR ||
				st.st_mode & S_IXGRP ||\
				st.st_mode & S_IXOTH ){//判断是否具有可执行权限
			//只要存在一个可执行权限，那么我们就说该文件是可执行的
			printf("可执行文件\n");
			cgi=1;	
		}
		if(cgi == 1){
			printf("path:%s\n",path);
			printf("query:%s\n",query);
			excu_cgi(sock,path,method,query);
		}
		else{
			printf("echo_html\n");
			printf("%s\n",path);
			echo_html(sock,path,st.st_size);
		}
	}
	close(sock);
	return 0;
}
