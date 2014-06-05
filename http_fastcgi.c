#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>


#include "http_request_r.h"
#include "http_response_r.h"
#include "http_fastcgi.h"

int fast_cgi_handle(struct http_request_r *r,http_response_r *o)
{
	int port = 9000;
	char *ip = "127.0.0.1";
	int requestId = 1;
	char name[512] = "SCRIPT_FILENAME";
	char value[512] = "";
	strcpy(value,r->path);
	
	int body_len,server_socket_fd,ret,record_len;
	unsigned char record[512],body[512],buf[512];
	memset(record,0,512);
	memset(body,0,512);

	server_socket_fd = fcgi_connect(ip,port);
	if(server_socket_fd<0)
	{
		return -1;
	}

	writeBeginBody(server_socket_fd,requestId);	
	
	writeFcgiParams(name,value,server_socket_fd,requestId);
	
	strcpy(name,"REQUEST_METHOD");
	strcpy(value,"GET");
	writeFcgiParams(name,value,server_socket_fd,requestId);

	struct FCGI_Header fcgi_header;
	fcgi_header = makeHeader(FCGI_PARAMS,requestId,0,0);
	memcpy(record,&fcgi_header,sizeof(fcgi_header));

        ret = write(server_socket_fd,record,sizeof(fcgi_header));
        if(ret<0)
        {
                perror("write");
                return -1;
        }
	
	fcgi_header = makeHeader(FCGI_STDIN,requestId,0,0);
        memcpy(record,&fcgi_header,sizeof(fcgi_header));
        memcpy(record+r->buf_size,r->buf,r->buf_size);

        ret = write(server_socket_fd,record,sizeof(fcgi_header));
        if(ret<0)
        {
                perror("write");
                return -1;
        }

	struct FCGI_Header responderHeader;
	struct FCGI_EndRequestBody fcgi_EndRequestBody; 
	char content[1024],tmp[128];
	int content_length;
	int contentLength;

	while(read(server_socket_fd,&responderHeader,sizeof(struct FCGI_Header))>0)
	{

		//printf("type:%d\n",responderHeader.type);		

		if(responderHeader.type==FCGI_STDOUT)
		{
			content_length = (int)(responderHeader.contentLengthB1<<8) + (int)(responderHeader.contentLengthB0);		
			//printf("content_length:%d\n",content_length);
			memset(content,0,sizeof(content));
			ret = read(server_socket_fd,content,content_length);
			if(ret<0)
			{
				perror("read");
			}
			o->content_length = content_length;
			o->body = malloc(content_length);
			memcpy(o->body,content,content_length);			

			if(responderHeader.paddingLength>0)
			{
				ret = read(server_socket_fd,tmp,responderHeader.paddingLength);
				if(ret<0)
				{
					perror("read");
				}
			}
		}
		
		if(responderHeader.type==FCGI_END_REQUEST)
		{
			content_length = (int)(responderHeader.contentLengthB1<<8) + (int)(responderHeader.contentLengthB0);
			ret = read(server_socket_fd,&fcgi_EndRequestBody,content_length);
			if(ret<0)
			{
				perror("read");	
			}

			fprintf(stdout,"\nendRequest:appStatus:%d,protocolStatus:%d\n",(fcgi_EndRequestBody.appStatusB3<<24)+(fcgi_EndRequestBody.appStatusB2<<16)+(fcgi_EndRequestBody.appStatusB1<<8)+(fcgi_EndRequestBody.appStatusB0),fcgi_EndRequestBody.protocolStatus);

			if(responderHeader.paddingLength>0)
                        {
                                ret = read(server_socket_fd,tmp,responderHeader.paddingLength);
                                if(ret<0)
                                {
                                        perror("read");
                                }
                        }
			close(server_socket_fd);
		}
	
		if(responderHeader.type==FCGI_STDERR)
                {
                        content_length = (responderHeader.contentLengthB1<<8) + (responderHeader.contentLengthB0);
                        memset(content,0,sizeof(content));
                        ret = read(server_socket_fd,content,content_length);
                        if(ret<0)
                        {
                                perror("read");
                        }
			printf("%s\n",content);
                        if(responderHeader.paddingLength>0)
                        {
                                ret = read(server_socket_fd,tmp,responderHeader.paddingLength);
                                if(ret<0)
                                {
                                        perror("read");
                                }
                        }
                }	
	}			
}

int fcgi_connect(char *ip,int port)
{
        int server_socket_fd,ret;

        struct sockaddr_in server_addr;
        memset(&server_addr,0,sizeof(server_addr));

        if( (server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        {
                perror("socket");
        	return -1;
	}

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(ip);
        server_addr.sin_port = htons(port);

        ret = connect(server_socket_fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
        if(ret<0)
        {
                perror("conenct");
        	return -1;
	}

	return server_socket_fd;
}

struct FCGI_Header makeHeader(int type,int requestId,int contentLength,int paddingLength)
{
	struct FCGI_Header fcgi_header;
	
	fcgi_header.version = (unsigned char)FCGI_VERSION_1;
	fcgi_header.type = (unsigned char)type;
	fcgi_header.requestIdB1 = (unsigned char)(requestId>>8 & 0x7f); 
	fcgi_header.requestIdB0 = (unsigned char)(requestId & 0x7f);
	fcgi_header.contentLengthB1 = (unsigned char)(contentLength>>8 & 0x7f); 
	fcgi_header.contentLengthB0 = (unsigned char)(contentLength & 0x7f);
	fcgi_header.paddingLength = (unsigned char)paddingLength;
	fcgi_header.reserved = 0;
	
	return fcgi_header;
}

struct FCGI_BeginRequestBody makeBeginBody(int flags)
{
	struct FCGI_BeginRequestBody fcgi_beginRequestBody;
	
	fcgi_beginRequestBody.roleB1 = (unsigned char)(FCGI_RESPONDER>>8 & 0x7f);
	fcgi_beginRequestBody.roleB0 = (unsigned char)(FCGI_RESPONDER & 0x7f);
	fcgi_beginRequestBody.flags = (unsigned char)flags;
	memset(fcgi_beginRequestBody.reserved,0,sizeof(fcgi_beginRequestBody.reserved));	
	
	return fcgi_beginRequestBody;
}

void makeFcgiParams(char *name,char *value,int *body_len,unsigned char *body)
{
	int name_len = strlen(name);
	int value_len = strlen(value);
	unsigned char *p = body;

	if(name_len<0x80)
	{
		*body++ = (unsigned char)name_len;
	}
	else
	{
		*body++ = (unsigned char)((name_len>>24) & 0x7f);
		*body++ = (unsigned char)((name_len>>16) & 0x7f);
		*body++ = (unsigned char)((name_len>>8) & 0x7f);
		*body++ = (unsigned char)(name_len & 0x7f);
	}

	if(value_len<0x80)
	{
		*body++ = (unsigned char)value_len;	
	}
	else
	{
		*body++ = (unsigned char)((value_len>>24) & 0x7f);
		*body++ = (unsigned char)((value_len>>16) & 0x7f);
		*body++ = (unsigned char)((value_len>>8) & 0x7f);
		*body++ = (unsigned char)(value_len & 0x7f);
	}

	while(*name!='\0')
	{
		*body++ = *name++;
	}
	
	while(*value!='\0')
	{
		*body++ = *value++;
	}

	*body_len = body-p;


} 

int writeFcgiParams(char *name,char *value,int server_socket_fd,int requestId)
{
	unsigned char body[512],record[512];
	int body_len,ret;
	struct FCGI_Header fcgi_header;

	memset(record,0,sizeof(record));
        memset(body,0,sizeof(body));

        makeFcgiParams(name,value,&body_len,&body[0]);
        fcgi_header = makeHeader(FCGI_PARAMS,requestId,body_len,0);

        memcpy(record,&fcgi_header,sizeof(fcgi_header));
        memcpy(record+sizeof(fcgi_header),&body[0],body_len);

        ret = write(server_socket_fd,record,sizeof(fcgi_header)+body_len);
        if(ret<0)
        {
                perror("write");
                return -1;
        }
	return ret;	
}

int writeBeginBody(int server_socket_fd,int requestId)
{
	unsigned char record[512];
	int ret;

	struct FCGI_BeginRequestBody fcgi_beginRequestBody;
        fcgi_beginRequestBody = makeBeginBody(1);

        struct FCGI_Header fcgi_header;
        fcgi_header = makeHeader(FCGI_BEGIN_REQUEST,requestId,sizeof(fcgi_beginRequestBody),0);

        memcpy(record,&fcgi_header,sizeof(fcgi_header));
        memcpy(record+sizeof(fcgi_header),&fcgi_beginRequestBody,sizeof(fcgi_beginRequestBody));

        ret = write(server_socket_fd,record,sizeof(fcgi_beginRequestBody)+sizeof(fcgi_beginRequestBody));
        if(ret<0)
        {
                perror("write");
                return -1;
        }

	return ret;
}

