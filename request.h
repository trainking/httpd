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

void construct_request(int sock);