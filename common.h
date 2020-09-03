/*request response 共有头文件*/

#define METHOD_GET   "GET"
#define METHOD_POST  "POST"
#define METHOD_PUT   "PUT"
#define METHOD_HEAD  "HEAD"
#define METHOD_OPTIONS  "OPTIONS"
#define METHOD_DELETE "DELETE"

#ifndef _ENTITY_E_
#define _ENTITY_E_
typedef struct _entity {
    char name[128];
    char value[128];
} Entity;
#endif

int get_line(int sock, char *buf, int size);
