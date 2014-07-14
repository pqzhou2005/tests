#define EVENT_NONE 0
#define EVENT_READABLE 1
#define EVENT_WRITEABLE 2

typedef struct http_event_t http_event_t;

typedef struct http_event_loop_t http_event_loop_t;

typedef void event_handler(http_event_t *);

struct http_event_t
{
	int fd;
	event_handler *read_handler;
	event_handler *write_handler;
	int mask;
	http_event_loop_t *event_loop;
};

struct http_event_loop_t
{
	int epfd;
	int event_type;
	http_event_t *http_events;
};

http_event_loop_t *http_event_create(int event_max);

int http_event_add(struct http_event_loop_t *event_loop,int fd,int mask,event_handler *read_hander,event_handler *write_handler);

int http_event_del(struct http_event_loop_t *event_loop,int fd,int mask);

int http_event_main(struct http_event_loop_t *ev_loop);
