#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <ctype.h>
#include "request.h"

#define ISspace(x) isspace((int)(x))

void construct_request(int sock)
{
    Request r;
    fd_set rset;  // IO检查读
    int maxfpl;   // 最大可读socket+1
    char buff[1024];  // 缓存区
    char method[7];  // 请求方法 最长的请求方法是DELETE+1 = 7
    size_t i;
    int n = 1;
    
    // select IO检查
    FD_ZERO(&rset);
    FD_SET(sock, &rset);
    maxfpl = sock + 1;
    select(maxfpl, &rset, NULL, NULL, NULL);
    
    //1. 读取第一行
    get_line(sock, buff, sizeof(buff));
    for (i=0; i < 6; i ++) {
        if (ISspace(buff[i]))
            break;
        method[i] = buff[i];
    }
    // 1.1 获取请求方法
    method[i] = '\0';
    strcpy(r.method, method);
    printf("method: %s\n", r.method);
    // 1.2 获取协议版本
}