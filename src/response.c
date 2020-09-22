#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include "include/response.h"

#define CASE_MSG(code, m)\
    case code:\
        msg = m;\
        break

/*
* 构造返回报文
* @param int status 状态码
* @param int client 客户端的socket描述符
* @param char *body 需要输出的内容
*/
void response(int status, int client, char *body)
{
    char* msg = "\0";
    char result[1024];  // 输出缓存区
    switch(status) {
        CASE_MSG(100, "Continue");
        CASE_MSG(101, "Switching Protocols");
        CASE_MSG(200, "ok");
        CASE_MSG(201, "Created");
        CASE_MSG(202, "Accepted");
        CASE_MSG(203, "Non-Authoritative Information");
        CASE_MSG(204, "No Content");
        CASE_MSG(205, "Reset Content");
        CASE_MSG(206, "Partial Content");
        CASE_MSG(300, "Multiple Choices");
        CASE_MSG(301, "Moved Permanently");
        CASE_MSG(302, "Found");
        CASE_MSG(303, "See Other");
        CASE_MSG(304, "Not Modified");
        CASE_MSG(305, "Use Proxy");
        CASE_MSG(306, "Unused");
        CASE_MSG(307, "Temporary Redirect");
        CASE_MSG(400, "Bad Request");
        CASE_MSG(401, "Unauthorized");
        CASE_MSG(402, "Payment Required");
        CASE_MSG(403, "Forbidden");
        CASE_MSG(404, "Not Found");
        CASE_MSG(405, "Method Not Allowed");
        CASE_MSG(406, "Not Acceptable");
        CASE_MSG(407, "Proxy Authentication Required");
        CASE_MSG(408, "Request Timeout");
        CASE_MSG(409, "Conflict");
        CASE_MSG(410, "Gone");
        CASE_MSG(411, "Length Required");
        CASE_MSG(412, "Precondition Failed");
        CASE_MSG(413, "Request Entity Too Large");
        CASE_MSG(414, "Request-URI Too Long");
        CASE_MSG(415, "Unsupported Media Type");
        CASE_MSG(416, "Requested Range Not Satisfiable");
        CASE_MSG(417, "Expectation Failed");
        CASE_MSG(500, "Internal Server Error");
        CASE_MSG(501, "Not Implemented");
        CASE_MSG(502, "Bad Gateway");
        CASE_MSG(503, "Service Unavailable");
        CASE_MSG(504, "Gateway Timeout");
        CASE_MSG(505, "HTTP Version Not Supported");
    }

    sprintf(result, "HTTP/1.1 %d %s\r\n", status, msg);
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    // 20x 返回正常结果
    if (status >= 200 && status <= 206) {
        sprintf(result, "%s\r\n", body);
    } else {
        sprintf(result, "%s\r\n", msg);
    }
    send(client, result, strlen(result), 0);
}