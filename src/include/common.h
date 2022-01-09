/*request response 共有头文件*/


#ifndef _HTTP_COMMONT_

#define _HTTP_COMMONT_
#define METHOD_GET   "GET"
#define METHOD_POST  "POST"
#define METHOD_PUT   "PUT"
#define METHOD_HEAD  "HEAD"
#define METHOD_OPTIONS  "OPTIONS"
#define METHOD_DELETE "DELETE"

#define true 1;
#define false 0;

typedef int BOOL;

typedef struct _entity {
    char name[128];
    char value[128];
} Entity;

#endif

int get_line(int sock, char *buf, int size);
u_int32_t ipv4_to_int(char *ip);
char* html_path();
