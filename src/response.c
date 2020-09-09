#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include "include/response.h"


/*
* 构造返回报文
* @param int status 状态码
* @param int client 客户端的socket描述符
* @param char *result 需要输出的内容
*/
void response(int status, int client, char *result)
{
    char* msg = "\0";
    switch(status) {
        case 100:
            msg = "Continue";
            break;
        case 101:
            msg = "Switching Protocols";
            break;
        case 200:
            msg = "ok";
            break;
        case 201:
            msg = "Created";
            break;
        case 202:
            msg = "Accepted";
            break;
        case 203:
            msg = "Non-Authoritative Information";
            break;
        case 204:
            msg = "No Content";
            break;
        case 205:
            msg = "Reset Content";
            break;
        case 206:
            msg = "Partial Content";
            break;
        case 300:
            msg = "Multiple Choices";
            break;
        case 301:
            msg = "Moved Permanently";
            break;
        case 302:
            msg = "Found";
            break;
        case 303:
            msg = "See Other";
            break;
        case 304:
            msg = "Not Modified";
            break;
        case 305:
            msg = "Use Proxy";
            break;
        case 306:
            msg = "Unused";
            break;
        case 307:
            msg = "Temporary Redirect";
            break;
        case 400:
            msg = "Bad Request";
            break;
        case 401:
            msg = "Unauthorized";
            break;
        case 402:
            msg = "Payment Required";
            break;
        case 403:
            msg = "Forbidden";
            break;
        case 404:
            msg = "Not Found";
            break;
        case 405:
            msg = "Method Not Allowed";
            break;
        case 406:
            msg = "Not Acceptable";
            break;
        case 407:
            msg = "Proxy Authentication Required";
            break;
        case 408:
            msg = "Request Timeout";
            break;
        case 409:
            msg = "Conflict";
            break;
        case 410:
            msg = "Gone";
            break;
        case 411:
            msg = "Length Required";
            break;
        case 412:
            msg = "Precondition Failed";
            break;
        case 413:
            msg = "Request Entity Too Large";
            break;
        case 414:
            msg = "Request-URI Too Long";
            break;
        case 415:
            msg = "Unsupported Media Type";
            break;
        case 416:
            msg = "Requested Range Not Satisfiable";
            break;
        case 417:
            msg = "Expectation Failed";
            break;
        case 500:
            msg = "Internal Server Error";
            break;
        case 501:
            msg = "Not Implemented";
            break;
        case 502:
            msg = "Bad Gateway";
            break;
        case 503:
            msg = "Service Unavailable";
            break;
        case 504:
            msg = "Gateway Timeout";
            break;
        case 505:
            msg = "HTTP Version Not Supported";
            break;
    }

    sprintf(result, "HTTP/1.1 %d %s\r\n", status, msg);
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "%s\r\n", msg);
    send(client, result, strlen(result), 0);
}