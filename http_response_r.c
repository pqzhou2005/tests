#include <string.h>
#include <sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>

#include "str/str.h"
#include "date/date.h"
#include "http_request_r.h"
#include "http_response_r.h"
#include "content_type.h"
#include "http_fastcgi.h"

int static_http_response_out(struct http_request_r *r,struct http_response_r *o)
{
	init_http_response(r,o);	

	int ret;
	ret = access(r->path,R_OK);
	if(ret==-1)
	{
		o->status_code = 404;		
	}
	
        if(o->status_code==404)
        {
                o->reason_phrase = "Not Found";
                strcpy(r->ext,"html");
                strcpy(r->path,"/data/tests/error_404.html");
        }

	struct stat stat_buf;
	ret = stat(r->path,&stat_buf);
	if(ret==-1)
	{
		perror("stat error");
	}

	if(strcmp(r->ext,"php")==0)
	{
		fast_cgi_handle(r,o);
	}
	else
	{
		readfile(r->path,o);
	}

	o->last_modified = gdate(&stat_buf.st_mtime);

	char *ext_point;
	ext_point = (char *)malloc(10);
	memset(ext_point,0,10);	

	printf("ext:%s\n",r->ext);
	sprintf(ext_point,".%s",r->ext);

	char *content_type;
	content_type = find_content_type(ext_point);

	char *c_type;
	c_type = (char *)malloc(50);
	memset(c_type,0,50);
	sprintf(c_type,"%s",content_type); 
	o->content_type = c_type;
	
	gen_http_response_buf(o);

	return 0;
}


int init_http_response(struct http_request_r *r,struct http_response_r *o)
{
	o->status_code = 200;

        o->http_version = (char *)malloc(10);
        if(o->http_version==NULL){
		printf("%s is error!\n","o http_version");	
	}
	memset(o->http_version,0,10);
	strcpy(o->http_version,"HTTP/1.1");

        o->server = (char *)malloc(12);
        if(o->server==NULL){
                printf("%s is error!\n","o server");  
        }
	memset(o->server,0,12);
        strcpy(o->server,"HTTP SERVER");

        o->reason_phrase = (char *)malloc(50);
	memset(o->reason_phrase,0,50);
	strcpy(o->reason_phrase,"OK");

	o->date = (char *)malloc(100);
	memset(o->date,0,100);
	time_t t = time((time_t *)NULL);
	o->date = gdate(&t);	

	o->content_type = (char *)malloc(50);
	memset(o->content_type,0,50);
	o->content_type = "text/html; charset=utf8";
						
	o->connection = (char *)malloc(20);
	memset(o->connection,0,20);
	o->connection = "keep-alive";

	o->last_modified = (char *)malloc(50);
	memset(o->last_modified,0,50);
	o->last_modified = "";

	o->accept_ranges = (char *)malloc(50);
	memset(o->accept_ranges,0,50);
	o->accept_ranges = "bytes";

	return 1;			
}

int gen_http_response_buf(struct http_response_r *o)
{
	printf("%s test...\n","test");
	char *status_line;
	int size=0,nsize=0;
	
	size = strlen(o->http_version)+strlen(o->reason_phrase)+8;

	nsize = size + nsize;
	status_line = (char *)malloc(size);
	memset(status_line,0,size);	

	sprintf(status_line,"%s %d %s\r\n",o->http_version,o->status_code,o->reason_phrase);

	char *server;
	size = strlen(o->server)+10;
	nsize = size + nsize;
	server = (char *)malloc(size);
	memset(server,0,size);
	
	sprintf(server,"Server: %s\r\n",o->server);
	
	char *date;
	size = strlen(o->date)+10;
	nsize = size + nsize;
	date = (char *)malloc(size);
	memset(date,0,size);
	
	sprintf(date,"Date: %s\r\n",o->date);

	char *content_type;
	size = strlen(o->content_type)+20;
	nsize = size + nsize;
	content_type = (char *)malloc(size);
	memset(content_type,0,size);

	sprintf(content_type,"Content-Type: %s\r\n",o->content_type);

	char *content_length;
	size = 30;
	nsize = size + nsize;
	content_length = (char *)malloc(size);
	memset(content_length,0,size);

	sprintf(content_length,"Content-Length: %d\r\n",o->content_length);

	char *last_modified;
	size = strlen(o->last_modified)+20;
	nsize = size + nsize;
	last_modified = (char *)malloc(size);
	memset(last_modified,0,size);

	sprintf(last_modified,"Last-Modified: %s\r\n",o->last_modified);

	
	char *connection;
	size = strlen(o->connection)+20;
	nsize = size + nsize;
	connection = (char *)malloc(size);
	memset(connection,0,size);

	sprintf(connection,"Connection: %s\r\n",o->connection);

	char *accept_ranges;
	size = strlen(o->accept_ranges)+20;
	nsize = size + nsize;
	accept_ranges = (char *)malloc(size);
	memset(accept_ranges,0,size);

	sprintf(accept_ranges,"Accept-Ranges: %s\r\n",o->accept_ranges);

	nsize = nsize + 2;
	o->buf = (char *)malloc(nsize);
	memset(o->buf,0,nsize);

	sprintf(o->buf,"%s%s%s%s%s%s%s%s\r\n",status_line,server,date,content_type,content_length,last_modified,connection,accept_ranges);
	o->size = strlen(o->buf);
	return 0;
}                                                                                                                                                                                                                                                                       

int readfile(char *path, struct http_response_r *o)
{
        FILE * fp;
        int size;

        fp = fopen(path,"r");
        if(fp==NULL)
        {
                o->body = "";
                o->content_length = 0;
		return -1;
        }

        fseek(fp,0,SEEK_END);
        size = ftell(fp);
	o->content_length = size;

	printf("size:%d\n",size);	
	
        o->body = malloc(size);
        memset(o->body,0,size);

        rewind(fp);
        fread(o->body,1,size,fp);
        fclose(fp);

        return 0;
}

	
