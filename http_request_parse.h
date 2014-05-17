typedef struct http_request_fields
{
	char *key;
	char *val;
}http_request_fields;

int http_request_parse(struct http_request_r *r,char p[1024]);
int http_request_header_parse(struct http_request_r *r);
int http_request_header_fields_parse(struct http_request_r *r);
int http_request_line_parse(struct http_request_r *r);
