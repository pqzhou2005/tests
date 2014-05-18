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

int static_http_response_out(struct http_request_r *r,struct http_response_r *o)
{
	int i,j,n,k,m,ret,period_pos=0,query_pos=0,slash_pos=0,ext_end_pos=0,ext_length=0;
	char *dir;
	char *file_name;
	char *ext;
	char ch;
	char *const_ext[7]={"png","jpg","gif","swf","zip","html","htm"};
	char *html_ext = "html";
	char *html = "index.html";

	printf("request_uri:%s %d\n",r->request_uri,strlen(r->request_uri));

	init_http_response(r,o);

	dir = (char *)malloc(100);
	memset(dir,0,100);
	
	file_name = (char *)malloc(120);
	memset(file_name,0,120);
	
	ext = (char *)malloc(10);
	memset(ext,0,10);

	n = strlen(r->request_uri);
	
	for(i=0;i<n;i++)
	{
		ch = r->request_uri[i];
		if(ch=='/')
		{
			slash_pos=i;
		}
		
		if(ch=='.')
		{
			period_pos=i;
		}
		
		if(ch=='?')
		{
			query_pos=i;
			break;
		}
	}
	
	strcpy(dir,"/data/tests/web/");
	if(slash_pos>0)
	{
		cstrcpy(dir,r->request_uri,0,slash_pos);
	}

	printf("%d %d %d\n",period_pos,query_pos,slash_pos);

	//ext check
	if(period_pos>0)
	{
		if(query_pos>0)
		{
			ext_end_pos = query_pos-1;
		}
		else
		{
			ext_end_pos = n;
		}
	
		for(j=0;j<7;j++)
		{
			m=0;
			i=period_pos+1;
			for(k=0;k<strlen(const_ext[j]);k++)
			{
				if(const_ext[j][k]==r->request_uri[i])
				{
					m++;	
				}
				i++;
			}

			printf("m:%d n:%d i:%d ext:%d\n",m,strlen(const_ext[j]),i,ext_end_pos);
	
			if(m==strlen(const_ext[j]) && i<=ext_end_pos)
			{
				strcpy(ext,const_ext[j]);
				break;
			}
		}

		if(strlen(ext)>0)
		{
			cstrcpy(file_name,r->request_uri,slash_pos+1,ext_end_pos-slash_pos-1);	
		}
		else
		{
			o->status_code = 404;	
		}
	
	}
	else
	{
		strcpy(ext,html_ext);
		strcpy(file_name,html);
	}
	
	if(o->status_code==200)
	{
		strcat(dir,file_name);
		
		ret = access(dir,R_OK);
		if(ret==-1)
		{
			perror("access error");
			o->status_code = 404;		
		}
	}
	printf("%d\n",o->status_code);

	if(o->status_code==404)
	{
		o->reason_phrase = "Not Found";
		strcpy(ext,"html");	
		strcpy(dir,"/data/tests/error_404.html");
	}


	struct stat stat_buf;
	ret = stat(dir,&stat_buf);
	if(ret==-1)
	{
		perror("stat error");
	}

	ret = readfile(dir,o);

	o->last_modified = gdate(&stat_buf.st_mtime);

	char *ext_point;
	ext_point = (char *)malloc(10);
	memset(ext_point,0,10);	

	printf("%s\n",ext);
	sprintf(ext_point,".%s",ext);
	printf("%s\n",ext_point);

	char *content_type;
	content_type = find_content_type(ext_point);
	printf("%s\n",content_type);

	char *c_type;
	c_type = (char *)malloc(50);
	memset(c_type,0,50);
	sprintf(c_type,"%s",content_type); 
	printf("%s\n",c_type);
	o->content_type = c_type;
	//strcat(o->content_type,";charset=utf8");	

	
	gen_http_response_buf(o);

	return 0;
}


int init_http_response(struct http_request_r *r,struct http_response_r *o)
{
	o->status_code = 200;

        o->http_version = (char *)malloc(10);
        memset(o->http_version,0,10);
	strcpy(o->http_version,"HTTP/1.1");

        o->server = (char *)malloc(12);
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
	char *status_line;
	int size,nsize=0;
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

	
