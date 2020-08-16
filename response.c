#include <stdio.h>
#include <strings.h>
#include <string.h>
#include "response.h"

void response_200(int client, char *result) {
    // RESPONSE _r = {
    //     code: 200,
    //     header: header,
    //     body: body
    // };
    
    sprintf(result, "HTTP/1.0 200 OK\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "ok\r\n");
    send(client, result, strlen(result), 0);
    // snprintf(result, sizeof(result), "%s\r\r", *header);
    // sprintf(result, "\r\n");
    // snprintf(result, sizeof(result), "%s\r\r", *body);
}