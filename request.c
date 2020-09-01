#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <ctype.h>
#include "request.h"

#define ISspace(x) isspace((int)(x))

int construct_request(int sock)
{
    Request r;
    fd_set rset;  // IO检查读
    int maxfpl;   // 最大可读socket+1
    char buff[1024];  // 缓存区
    char method[7];  // 请求方法 最长的请求方法是DELETE+1 = 7
    char path[512];  // 请求路由
    char query[512];  // 请求参数
    char version[10];
    size_t i;
    int j,k;

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
    printf("i: %d method: %s\n", (int)i, r.method);
    printf("buff: %s\n", buff);
    // 1.2 请求路径（路由）
    i++;
    int _qflag = 0;
    // int _n =0 ;
    // int _p = 0;
    j = 0; k =0;
    while(!ISspace(buff[i]) && (i < sizeof(buff) - 1))
    {
        if (buff[i] == '?') {
            _qflag = 1;
            i++;     // 增一位忽略掉 ?
        }

        if (_qflag) {
            query[j] = buff[i];
            j ++;
        } else {
            path[k] = buff[i];
            k++;
        }
        i++;
    }
    query[j + 1] = '\0';
    path[k + 1] = '\0';
    printf("\n");
    printf("path: %s\n", path);
    printf("query: %s\n", query);
    printf("i: %d\n", (int)i);
    i++;
    j = 0;
    while(!ISspace(buff[i]) && (i < sizeof(buff) - 1))
    {
        version[j] = buff[i];
        j ++;
        i ++;
    }
    printf("version：%s\n", version);
    // 验证协议版本
    if (strcasecmp(version, "HTTP/1.1") != 0)
        return 505;
    return 0;
}