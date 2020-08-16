/*response 返回头文件*/

#include "common.h"

#ifndef _RESPONSE_H_
#define _RESPONSE_H_
typedef struct response {
    int code;
    HENTITY header[10];   // 先假设有10个头
    char *body;
} RESPONSE;
#endif

char* response_200(char *body);