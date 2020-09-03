#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include "response.h"

void response_100(int client, char *result);
void response_101(int client, char *result);
void response_200(int client, char *result);
void response_201(int client, char *result);
void response_202(int client, char *result);
void response_203(int client, char *result);
void response_204(int client, char *result);
void response_205(int client, char *result);
void response_206(int client, char *result);
void response_505(int client, char *result);

// 100 Continue
void response_100(int client, char *result) {
    sprintf(result, "HTTP/1.1 100 Continue\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Continue\r\n");
    send(client, result, strlen(result), 0);
}

// 101 Switching Protocols
void response_100(int client, char *result) {
    sprintf(result, "HTTP/1.1 101 Switching Protocols\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Switching Protocols\r\n");
    send(client, result, strlen(result), 0);
}

// 200 OK
void response_200(int client, char *result) {
    sprintf(result, "HTTP/1.1 200 OK\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "ok\r\n");
    send(client, result, strlen(result), 0);
}

// 201 Created 资源已经创建
void response_201(int client, char *result) {
    sprintf(result, "HTTP/1.1 201 OK\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Created\r\n");
    send(client, result, strlen(result), 0);
}

// 202 Accepted 接受请求，但处理可能还未完成
void response_202(int client, char *result) {
    sprintf(result, "HTTP/1.1 202 Accepted\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Accepted\r\n");
    send(client, result, strlen(result), 0);
}

// 203 Non-Authoritative Information
void response_203(int client, char *result) {
    sprintf(result, "HTTP/1.1 203 Non-Authoritative Information\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Non-Authoritative Information\r\n");
    send(client, result, strlen(result), 0);
}

// 204 No Content 返回内容没有body
void response_204(int client, char *result) {
    sprintf(result, "HTTP/1.1 204 No Content\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "No Content\r\n");
    send(client, result, strlen(result), 0);
}

// 205 Reset Content 应重置
void response_205(int client, char *result) {
    sprintf(result, "HTTP/1.1 204 Reset Content\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Reset Content\r\n");
    send(client, result, strlen(result), 0);
}

// 206 Partial Content 请求头里包含Range字段，读取部分
void response_206(int client, char *result) {
    sprintf(result, "HTTP/1.1 Partial Content\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Partial Content\r\n");
    send(client, result, strlen(result), 0);
}

// 505 HTTP Version Not Supported
void response_505(int client, char *result) {
    sprintf(result, "HTTP/1.1 505 HTTP Version Not Supported\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "505 HTTP Version Not Supported\r\n");
    send(client, result, strlen(result), 0);
}

// 构造返回
void response(int status, int client, char *result)
{
    switch(status) {
        case 200:
            response_200(client, result);
            break;
        case 505:
            response_505(client, result);
            break;
    }
}