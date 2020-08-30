#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include "request.h"

void construct_request(int sock)
{
    fd_set rset;  // IO检查读
    int maxfpl;   // 最大可读socket+1
    char buff[1024];  // 缓存区
    int n = 1;
    int i = 1;
    
    // select IO检查
    FD_ZERO(&rset);
    FD_SET(sock, &rset);
    maxfpl = sock + 1;
    select(maxfpl, &rset, NULL, NULL, NULL);
    while((n > 0) && strcmp("\n", buff))
    {
        n = get_line(sock, buff, sizeof(buff));
        printf("Line %d: %s\n", i, buff);
        i++;
    }
}