typedef struct FCGI_Header {
     unsigned char version;
     unsigned char type;
     unsigned char requestIdB1;
     unsigned char requestIdB0;
     unsigned char contentLengthB1;
     unsigned char contentLengthB0;
     unsigned char paddingLength;
     unsigned char reserved;
} FCGI_Header;

typedef struct FCGI_BeginRequestBody{
     unsigned char roleB1;
     unsigned char roleB0;
     unsigned char flags;
     unsigned char reserved[5];
} FCGI_BeginRequestBody;

typedef struct FCGI_EndRequestBody{
     unsigned char appStatusB3;
     unsigned char appStatusB2;
     unsigned char appStatusB1;
     unsigned char appStatusB0;
     unsigned char protocolStatus;
     unsigned char reserved[3];
} FCGI_EndRequestBody;


#define FCGI_VERSION_1           1

#define FCGI_BEGIN_REQUEST       1
#define FCGI_ABORT_REQUEST       2
#define FCGI_END_REQUEST         3
#define FCGI_PARAMS              4
#define FCGI_STDIN               5
#define FCGI_STDOUT              6
#define FCGI_STDERR              7
#define FCGI_DATA                8
#define FCGI_GET_VALUES          9
#define FCGI_GET_VALUES_RESULT  10
#define FCGI_UNKNOWN_TYPE       11

#define FCGI_RESPONDER  1
#define FCGI_AUTHORIZER 2
#define FCGI_FILTER     3

int fast_cgi_handle(struct http_request_r *r,struct http_response_r *o);
int fcgi_connect(char *ip,int port);
struct FCGI_Header makeHeader(int type,int requestId,int contentLength,int paddingLength);
struct FCGI_BeginRequestBody makeBeginBody(int flags);
void makeFcgiParams(char *name,char *value,int *body_len,unsigned char *body);
int writeFcgiParams(char *name,char *value,int fd,int requestId);
int writeBeginBody(int server_socket_fd,int requestId);
