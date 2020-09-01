#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include "response.h"

void response_505(int client, char *result);
void response_200(int client, char *result);

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