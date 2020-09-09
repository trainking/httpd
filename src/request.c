#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <ctype.h>
#include <malloc.h>
#include "include/request.h"

#define ISspace(x) isspace((int)(x))

int entity_query(Entity* en, char* query, size_t maxlen);
int entity_header(Entity* en, char* header, size_t maxlen);

/*
* 构造请求结构体
* @param int sock socket描述符
* @return 
*/
int construct_request(int sock)
{
    Request r;
    fd_set rset;  // IO检查读
    int maxfpl;   // 最大可读socket+1
    char buff[1024];  // 缓存区
    int numchars;     // 缓存区取出的内容大小
    char method[7];  // 请求方法 最长的请求方法是DELETE+1 = 7
    char path[512];  // 请求路由
    char query[512];  // 请求参数
    char version[10];
    int rBody = 0;   // 是否读取body
    int content_length = -1;  // body长度
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
    // 1.3 请求query参数
    if (_qflag == 1) {
        eQuery = (Entity *) malloc(sizeof(Entity));
        int _c = entity_query(eQuery, query, (size_t)(j + 1));
        // 加入query
        if (_c > 0) {
            r.hasQuery = true;
            r.query = eQuery;
        }
    } else {
        r.hasQuery = false;
    }
    // 1.4 HTTP 版本信息
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
    // 2.1 取出header
    numchars = get_line(sock, buff, sizeof(buff));
    while ((numchars > 0) && strcmp("\n", buff) != 0)
    {
        Entity* _en = (Entity*)malloc(sizeof(Entity));
        if (entity_header(_en, buff, (size_t)(numchars + 1))) {
            // 检查各项header
            //printf("Entity-name:%s，value:%s\n", _en->name, _en->value);
            if (strcasecmp(_en->name, "Content-Length") == 0) {
                // body内容长度
                content_length = atoi(_en->value);
            }
        }
        numchars = get_line(sock, buff, sizeof(buff));
    }
    // 3.1 取出请求body
    if (rBody && content_length > 0) {
        // TODO
    }
    return 0;
}

/*
* 解析query参数
* @param Entity *en Entity的数组指针，解析好的请求参数
* @param char *query query参数的数组指针
* @param size_t maxlen query的内容长度
* @return int 放回en的内容长度
*/
int entity_query(Entity* en, char* query, size_t maxlen)
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

/*
* 读取header内容
* @param Entity* en 取出的头文件Entity
* @param char* header 缓存
* @param size_t maxlen 内容长度
* @return int 取出的header个数
*/
int entity_header(Entity* en, char* header, size_t maxlen)
{
    size_t i = 0;
    int v_flag = 0;
    int j = 0;
    int k = 0;
    char name[128];
    char value[128];
    while (i < maxlen) {
        if (header[i] == ':' && v_flag == 0) {
            v_flag = 1;
            i ++;
        }
        if (header[i] == '\0') {
            name[j] = '\0';
            value[k] = '\0';
            strcpy(en->name, name);
            strcpy(en->value, value);
            return 1;
        }
        if (v_flag == 1 && !ISspace(header[i])) {
            value[k] = header[i];
            k ++;
        }
        if (v_flag == 0 && !ISspace(header[i])) {
            name[j] = header[i];
            j ++;
        }
        i++;
    }
    return 0;
}