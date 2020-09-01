/*request 请求头文件*/
#include "common.h"

#ifndef __REQUEST_H_
#define __REQUEST_H_
typedef struct _request {
    char method[7];
    char *header;
    char *body;
} Request;
#endif

/*
* 构造请求结构
* 返回0构造正常，大于0则是响应码，做出响应
*/
int construct_request(int sock);