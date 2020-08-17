#include <stdio.h>
#include <strings.h>
#include <string.h>
#include "response.h"

void response_200(int client, char *result) {
    sprintf(result, "HTTP/1.0 200 OK\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "Content-type: text/html\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "\r\n");
    send(client, result, strlen(result), 0);
    sprintf(result, "ok\r\n");
    send(client, result, strlen(result), 0);
}