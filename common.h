/*request response 共有头文件*/

#define METHOD_GET   "GET"
#define METHOD_POST  "POST"
#define METHOD_PUT   "PUT"
#define METHOD_HEAD  "HEAD"
#define METHOD_OPTIONS  "OPTIONS"
#define METHOD_PATCH "PATCH"

// #ifndef _HEADER_E_
// #define _HEADER_E_
// typedef struct header_entity {
//     char *name;
//     char *value;
// } HENTITY;
// #endif

int get_line(int sock, char *buf, int size);
