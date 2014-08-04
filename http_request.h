#include <stdio.h>

typedef struct http_request_t
{
	int fd;
	
	int request_header_fields_pos;

        char *method;
        
	char *protocol;
        
	char *request_uri;

        unsigned int content_length;
        
	int content_pos;

        char *content;

        int is_end;

        int pos;

        char *ext;

        char *path;	

}http_request_t;

int main(int args,char *argv)
{
	
}
