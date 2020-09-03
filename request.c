#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <ctype.h>
#include <malloc.h>
#include "request.h"

#define ISspace(x) isspace((int)(x))

int entity_query(Entity *en, char *query, size_t maxlen);

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
    int rBody = 0;   // 是否读取body
    Entity *eQuery;
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
    if (strcasecmp(method, METHOD_POST) == 0)
        rBody = 1;
    if (strcasecmp(method, METHOD_PUT) == 0)
        rBody = 1;
    if (strcasecmp(method, METHOD_DELETE) == 0)
        rBody = 1;
    // 1.2 请求路径（路由）
    i++;
    int _qflag = 0;
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
    // 加入path
    strcpy(r.path, path);
    if (_qflag == 1) {
        eQuery = (Entity *) malloc(sizeof(Entity));
        int _c = entity_query(eQuery, query, (size_t)(j + 1));
        // TODO 保存取出的参数
        printf("_c: %d\n", _c);
    }
    i++;
    j = 0;
    while(!ISspace(buff[i]) && (i < sizeof(buff) - 1))
    {
        version[j] = buff[i];
        j ++;
        i ++;
    }
    // 验证协议版本
    if (strcasecmp(version, "HTTP/1.1") != 0)
        return 505;
    // 取出请求body
    if (rBody) {
        // TODO
    }
    return 0;
}

// 取出请求参数
int entity_query(Entity *en, char *query, size_t maxlen)
{
    int n = 1;
    size_t i = 0;
    while(!ISspace(query[i]) && (i < maxlen - 1)) {
        if (query[i] == '&')
            n ++;
        i ++;
    }
    en = (Entity *) malloc(n * sizeof(Entity));
    n = 0;
    i = 0;
    char name[128];
    char value[128];
    int v_flag = 0;
    int j = 0;
    int k = 0;
    while(i < maxlen) {
        if (query[i] == '=') {
            v_flag = 1;
            i ++;
            k = 0;
        }
        if (query[i] == '&' || query[i] == '\0') {
            v_flag = 0;
            name[j] = '\0';
            value[k] = '\0';
            Entity _item;
            strcpy(_item.name, name);
            strcpy(_item.value, value);
            en[n] = _item;
            bzero(&name, sizeof(name));
            bzero(&value, sizeof(value));
            j = 0;
            n ++;
            i ++;
            continue;
        }
        if (v_flag == 1) {
            value[k] = query[i];
            k ++;
        } else {
            name[j] = query[i];
            j ++;
        }
        
        i ++;
    }
    return n;
}