/*response 返回头文件*/

#include "common.h"

#ifndef _RESPONSE_H_
#define _RESPONSE_H_
typedef struct _response {
    int code;
    char *header;   // 先假设有10个头
    char *body;
} RESPONSE;
#endif

int response(int status, int client);
