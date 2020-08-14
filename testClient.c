#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(void) {
    
    int client_sock = -1;
    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_sock == -1) {
        printf("socket FAIL!");
        exit(1);
    }

    struct sockaddr_in name;
    bzero(&name, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(4000);
    name.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(client_sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
        printf("connect FAIL!");
        exit(1);
    }
    char sendline[1024], recvline[1024];
    snprintf(sendline, sizeof(sendline), "send0 %d", client_sock);
    write(client_sock, sendline, strlen(sendline));

    read(client_sock, recvline,sizeof(recvline));

    printf("DEBUG: recv data -- %s\n", recvline);

    bzero(sendline, sizeof(sendline));
    bzero(recvline, sizeof(recvline));
    close(client_sock);

    return 0;
}