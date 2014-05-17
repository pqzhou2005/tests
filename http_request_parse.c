#include <string.h>
#include <stdio.h>

#include "http_request_r.h"
#include "http_request_parse.h"
#include "math/math.h"

int http_request_parse(struct http_request_r *r,char p[1024])
{
	int n,i,header_length=0;
	char ch;

	printf("%s\n",p);
	
	strcat(r->buf,p);	
	n = strlen(r->buf);
	for(i=0;i<n;i++)
	{
		ch = r->buf[i];
		//printf("%02x ",ch);
		if(ch=='\r' && n>i+3)
		{
			if(r->buf[i+1]=='\n' && r->buf[i+2]=='\r' && r->buf[i+3]=='\n')
			{
				header_length = i+4;
				break;
			}
		}
	}

	if(header_length>0)
	{
		//parse header
		http_request_header_parse(r);
	}

	return 0;
}

int http_request_header_parse(struct http_request_r *r)
{
	int i,j,n,is_c=1,m=0,content_len;
	char ch;	
	char *content_length = "Content-Length";
	char content_len_str[12]={0};

	n = strlen(r->buf);
	
	for(i=0;i<n;i++)
	{
		ch = r->buf[i];
		if(ch=='\r' && r->buf[i+1]=='\n')
		{
			for(j=0;j<n;j++)
			{
				
				if(j<strlen(content_length) && r->buf[i+2+j]!=content_length[j])
				{
					is_c = 0;
					break;
				}
		
				if(r->buf[i+2+j]=='\r' && r->buf[i+3+j]=='\n')
				{
					break;
				}

				if(is_c == 1 && j>strlen(content_length) && ch>=48 && ch<=57)
				{
					content_len_str[m] = ch; 		
					m++;
				}
	
			}
			
		}		

	}

	printf("%d\n",m);
	printf("%s\n",content_len_str);
	content_len = chtoint(content_len_str);
	printf("%d\n",content_len);
}

int http_request_line_parse(struct http_request_r *r)
{
	
}

int http_request_header_fields_parse(struct http_request_r *r)
{
	
}

