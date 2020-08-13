/*引入头文件*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

/*函数声明*/
void error_die(const char *sc);
int startup(u_short *port);

/*错误退出，输出一条错误信息*/
void error_die(const char *sc)
{
    perror(sc);
    exit(1);
}

/*
* 启动http服务
* TCP服务器启动函数过程：socket() -> bind() -> listen() -> accept() -> 阻塞到客户端请求到来
*/
int startup(u_short *port)
{
    int httpd = 0;   // sockfd 套接字描述符
    struct sockaddr_in name;  // 协议地址结构体
    // 1. socket
    httpd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (httpd == -1)
        error_die("socket Fail!");
    // 2. bind
    // 2.1 初始化sockaddr_in (sockaddr_in时sockaddr的进化版本，将协议 端口 地址三者分离)
    // memset(&name, 0, sizeof(name));
    bzero(&name, sizeof(name));
    name.sin_family = AF_INET;   // 协议，指定IPV4
    name.sin_port = htons(*port);  // 端口， htons转换为大端序
    name.sin_addr.s_addr = htonl(INADDR_ANY);  // 地址，INADDR_ANY = 0.0.0.0
    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)  // bind 成功返回0，失败-1
        error_die("bind Fail!");
    // 2.2 以port=0 bind的地址会动态分配一个端口号，通过getsockname取出来
    if (*port ==0)
    {
        socklen_t name_len = sizeof(name);
        if (getsockname(httpd, (struct sockaddr *)&name, &name_len) < 0)
            error_die("getsockname Fail!");
        // 取出数据是网络序需转换
        *port = ntohs(name.sin_port);
    }
    // 3. listen
    /*
    * 5 是4.2BSD规定的最大数
    * 此数是套接字的未完成队列和已完成队列总和
    * 概念模糊，不同系统有不同实现，不要为0，0在不同系统又特殊含义
    */
    if (listen(httpd, 5) < 0)
        error_die("listen Fail!");
    // 4. accept
    return 0;
}

/*
* 主入口
*/
int main(void)
{
    u_short port = 4000;
    startup(&port);
    return 0;
}