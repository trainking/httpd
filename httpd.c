/* J. David's webserver */
/* This is a simple webserver.
 * Created November 1999 by J. David Blackstone.
 * CSE 4344 (Network concepts), Prof. Zeigler
 * University of Texas at Arlington
 */
/* This program compiles for Sparc Solaris 2.6.
 * To compile for Linux:
 *  1) Comment out the #include <pthread.h> line.
 *  2) Comment out the line that defines the variable newthread.
 *  3) Comment out the two lines that run pthread_create().
 *  4) Uncomment the line that runsaccept_request().
 *  5) Remove -lsocket from the Makefile.
 */
 /**
 * 头文件定义引用的类型，和调用的函数
 * + stdio.h 定义了C99标准的类型，如int_32_t表示32位有符号整数类型，是int的别名
 */
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "response.h"

#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"
#define STDIN   0
#define STDOUT  1
#define STDERR  2

#define METHOD_GET   "GET"
#define METHOD_POST  "POST"
#define METHOD_PUT   "PUT"
#define METHOD_HEAD  "HEAD"
#define METHOD_OPTIONS  "OPTIONS"
#define METHOD_PATCH "PATCH"

void accept_request(void *);
void cat(int, FILE *);
void error_die(const char *);
void execute_cgi(int, const char *, const char *, const char *);
int get_line(int, char *, int);
void headers(int, const char *);
void not_found(int);
void serve_file(int, const char *);
int startup(u_short *);

/**********************************************************************/
/* A request has caused a call to accept() on the server port to
 * return.  Process the request appropriately.
 * Parameters: the socket connected to the client */
/**********************************************************************/
/**处理请求**/
void accept_request(void *arg)
{
    int client = *(int*)arg;
    /**建立一个缓存区获取数据**/
    char buf[1024];
    /**size_t 是一个无符号长整型，它是用来记录一个大小的类型**/
    size_t numchars;
    /**HTTP请求的几个重要数据，请求方法，URL，路径**/
    char method[255];
    char url[255];
    char path[512];
    size_t i, j;
    struct stat st;
    int cgi = 0;      /* becomes true if server decides this is a CGI
                       * program */
    char *query_string = NULL;

    /**获取http请求报文的第一行**/
    numchars = get_line(client, buf, sizeof(buf));
    i = 0; j = 0;
    // 取出请求方法
    while (!ISspace(buf[i]) && (i < sizeof(method) - 1))
    {
        method[i] = buf[i];
        i++;
    }
    j=i;
    method[i] = '\0';

    /**strcasecmp判断字符串相等**/
    /**这步骤应该是用来判断方法是否实现的，但是使用&&符号，第一眼看过去会觉得懵逼，这个短路的特性会不能实现**/
    /**这里利用strcasecmp的特性 ，如果两个参数相等，则返回0，否则会比较，如果第一个参数比第二参数大，则返回大于0的数，反之则返回小于0的数**/
    if (strcasecmp(method, "GET") && strcasecmp(method, "POST") && strcasecmp(method, "PUT"))
    {
        response_unimplemented(client);
        return;
    }

    if (strcasecmp(method, "POST") == 0)
        cgi = 1;

    i = 0;
    while (ISspace(buf[j]) && (j < numchars))
        j++;
    while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < numchars))
    {
        url[i] = buf[j];
        i++; j++;
    }
    // 在URL的最后加上一个\0用来判断
    url[i] = '\0';

    if (strcasecmp(method, "GET") == 0)
    {
        // 通过指针来便利数组
        query_string = url;
        while ((*query_string != '?') && (*query_string != '\0'))
            query_string++;
        //定位到"?"出现的未知，将cgi设为true，并且这一位用'\0'代替
        if (*query_string == '?')
        {
            cgi = 1;
            *query_string = '\0';
            query_string++;
        }
    }

    sprintf(path, "htdocs%s", url);
    // 找出最后路由的文件地址，如果没有则指定到index.html
    if (path[strlen(path) - 1] == '/')
        strcat(path, "index.html");
    // 文件未找到则返回404
    if (stat(path, &st) == -1) {
        while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
            numchars = get_line(client, buf, sizeof(buf));
        not_found(client);
    }
    else
    {
        // 正常响应
        // 判断文件如果是文件夹则重定向index
        if ((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(path, "/index.html");
        // 判断文件是否有用户，用户组，其他人是否有执行权限，有则执行cgi为1
        if ((st.st_mode & S_IXUSR) ||
                (st.st_mode & S_IXGRP) ||
                (st.st_mode & S_IXOTH)    )
            cgi = 1;
        // cgi为空的情况下，拼装报文响应
        if (!cgi)
            // 发送静态报文
            serve_file(client, path);
        else
            // 执行cgi
            execute_cgi(client, path, method, query_string);
    }

    close(client);
}


/**********************************************************************/
/* Put the entire contents of a file out on a socket.  This function
 * is named after the UNIX "cat" command, because it might have been
 * easier just to do something like pipe, fork, and exec("cat").
 * Parameters: the client socket descriptor
 *             FILE pointer for the file to cat */
/**********************************************************************/
void cat(int client, FILE *resource)
{
    char buf[1024];

    fgets(buf, sizeof(buf), resource);
    while (!feof(resource))
    {
        send(client, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), resource);
    }
}

/**********************************************************************/
/* Print out an error message with perror() (for system errors; based
 * on value of errno, which indicates system call errors) and exit the
 * program indicating an error. */
/**********************************************************************/
void error_die(const char *sc)
{
    perror(sc);
    exit(1);
}

/**********************************************************************/
/* Execute a CGI script.  Will need to set environment variables as
 * appropriate.
 * Parameters: client socket descriptor
 *             path to the CGI script */
/**********************************************************************/
// 执行cgi
void execute_cgi(int client, const char *path,
        const char *method, const char *query_string)
{
    char buf[1024];   // 缓冲区
    int cgi_output[2];   // cgi的输出
    int cgi_input[2];    // cgi的输入
    pid_t pid;
    int status;
    int i;
    char c;
    int numchars = 1;
    int content_length = -1;

    buf[0] = 'A'; buf[1] = '\0';
    if (strcasecmp(method, "GET") == 0)
        while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
            numchars = get_line(client, buf, sizeof(buf));
    else if (strcasecmp(method, "POST") == 0) /*POST*/
    {
        // post 请求从body中读出数据
        numchars = get_line(client, buf, sizeof(buf));
        while ((numchars > 0) && strcmp("\n", buf))
        {
            buf[15] = '\0';
            if (strcasecmp(buf, "Content-Length:") == 0)
                content_length = atoi(&(buf[16]));
            numchars = get_line(client, buf, sizeof(buf));
        }
        if (content_length == -1) {
            response_400(client);
            return;
        }
    }
    else if (strcasecmp(method, "PUT") == 0)
    {
        // put 从body中读出数据
        numchars = get_line(client, buf, sizeof(buf));
        while ((numchars > 0) && strcmp("\n", buf))
        {
            buf[15] = '\0';
            if (strcasecmp(buf, "Content-Length:") == 0)
                content_length = atoi(&(buf[16]));
            numchars = get_line(client, buf, sizeof(buf));
        }
        if (content_length == -1) {
            response_400(client);
            return;
        }
    }
    else/*TODO 实现其他method*/
    {
    }


    if (pipe(cgi_output) < 0) {
        response_500(client);
        return;
    }
    if (pipe(cgi_input) < 0) {
        response_500(client);
        return;
    }

    if ( (pid = fork()) < 0 ) {
        response_500(client);
        return;
    }
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    if (pid == 0)  /* child: CGI script */
    {
        char meth_env[255];
        char query_env[255];
        char length_env[255];

        dup2(cgi_output[1], STDOUT);
        dup2(cgi_input[0], STDIN);
        close(cgi_output[0]);
        close(cgi_input[1]);
        sprintf(meth_env, "REQUEST_METHOD=%s", method);
        putenv(meth_env);
        if (strcasecmp(method, "GET") == 0) {
            sprintf(query_env, "QUERY_STRING=%s", query_string);
            putenv(query_env);
        }
        else {   /* POST */
            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(length_env);
        }
        execl(path, NULL);
        exit(0);
    } else {    /* parent */
        close(cgi_output[1]);
        close(cgi_input[0]);
        if (strcasecmp(method, "POST") == 0)
            for (i = 0; i < content_length; i++) {
                recv(client, &c, 1, 0);
                write(cgi_input[1], &c, 1);
            }
        while (read(cgi_output[0], &c, 1) > 0)
            send(client, &c, 1, 0);

        close(cgi_output[0]);
        close(cgi_input[1]);
        waitpid(pid, &status, 0);
    }
}

/**********************************************************************/
/* Get a line from a socket, whether the line ends in a newline,
 * carriage return, or a CRLF combination.  Terminates the string read
 * with a null character.  If no newline indicator is found before the
 * end of the buffer, the string is terminated with a null.  If any of
 * the above three line terminators is read, the last character of the
 * string will be a linefeed and the string will be terminated with a
 * null character.
 * Parameters: the socket descriptor
 *             the buffer to save the data in
 *             the size of the buffer
 * Returns: the number of bytes stored (excluding null) */
/**********************************************************************/
/**从一个socke中获取一行数据，返回这行数据大小，并把它写入到buf缓存区中**/
int get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;

    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';

    return(i);
}

/**********************************************************************/
/* Return the informational HTTP headers about a file. */
/* Parameters: the socket to print the headers on
 *             the name of the file */
/**********************************************************************/
void headers(int client, const char *filename)
{
    char buf[1024];
    (void)filename;  /* could use filename to determine file type */

    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Give a client a 404 not found status message. */
/**********************************************************************/
// 404未找到响应
void not_found(int client)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "your request because the resource specified\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Send a regular file to the client.  Use headers, and report
 * errors to client if they occur.
 * Parameters: a pointer to a file structure produced from the socket
 *              file descriptor
 *             the name of the file to serve */
/**********************************************************************/
void serve_file(int client, const char *filename)
{
    FILE *resource = NULL;
    int numchars = 1;
    char buf[1024];

    buf[0] = 'A'; buf[1] = '\0';
    while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
        numchars = get_line(client, buf, sizeof(buf));

    resource = fopen(filename, "r");
    if (resource == NULL)
        not_found(client);
    else
    {
        headers(client, filename);
        cat(client, resource);
    }
    fclose(resource);
}

/**********************************************************************/
/* This function starts the process of listening for web connections
 * on a specified port.  If the port is 0, then dynamically allocate a
 * port and modify the original port variable to reflect the actual
 * port.
 * Parameters: pointer to variable containing the port to connect on
 * Returns: the socket */
/**********************************************************************/
/**开启函数,监听端口，整个步骤就是寻址，然后建立监听的过程，开启服务端进程的过程**/
int startup(u_short *port)
{
    int httpd = 0;
    /**在AF_INET中，套接字地址用sockadd_in表示，sockaddr是通用的地址结构**/
    /**这个结构体包含三个参数，sin_family协议类型，sin_port端口，sin_addrIPV4协议地址**/
    /**sin_addr是一个结构体，只有一个成员s_addr**/
    struct sockaddr_in name;

    /**建立一个套接字，AF_INET与PF_INET一致，表示IPv4因特网网域**/
    /**第二个参数是确定套接字类型，SOCK_STREAM 是有序的，可靠的，双向的，面向连接的字节流，这符合http连接的特征**/
    /**第三个参数表示选定的协议，默认为0，表示为默认协议，如果可以使用多个协议时，通过这个参数来控制协议的选择，PF_INET中，默认协议为TCP**/
    httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd == -1)
        error_die("socket");
    /**清空sockaddr_in的缓存区，通过给这个值赋予0**/
    memset(&name, 0, sizeof(name));
    /**给sockaddr_in重新赋值**/
    /**协议族**/
    name.sin_family = AF_INET;
    /**字节序转换，将 *port 转换为网络字节序表示的16位整数**/
    /**端口，这个in_port_t的定义，是uint16_t，即2字节的无符号整型**/
    /**字节序的转换，因为TCP网络用的大端序，但大部分主机都是小端序的，所以为了解决字节序的问题，引入ht**()主机序转网络序，引入nt**()网络序转主机序**/
    name.sin_port = htons(*port);
    /**字节序转换，将 INADDR_ANY转换为网络字节序的32位整数*/
    /**这个s_addr的定义in_addr_t，是uint32_t，即4字节无符号整型**/
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
        error_die("bind");
    if (*port == 0)  /* if dynamically allocating a port */
    {
        socklen_t namelen = sizeof(name);
        if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
            error_die("getsockname");
        /**将网络字节序转换为主机字节序 16位**/
        *port = ntohs(name.sin_port);
    }
    if (listen(httpd, 5) < 0)
        error_die("listen");
    return(httpd);
}

/**********************************************************************/
/**主入口**/
int main(void)
{
    int server_sock = -1;
    u_short port = 4000;
    int client_sock = -1;
    struct sockaddr_in client_name;
    socklen_t  client_name_len = sizeof(client_name);
    pthread_t newthread;

    server_sock = startup(&port);  /**监听端口**/
    printf("httpd running on port %d\n", port);

    while (1)
    {
        /**accpet 函数用来获取连接并建立连接，其中第一个参数，传入原始套接字，即服务器套接字，第二个参数是获取地址的缓存区，建立到sockaddr_in，第三个参数是地址的长度**/
        /**返回时，会更新缓存区，保存地址，更新第三个参数，反应参数长度**/
        /**这一步是服务端向客户端发起请求，建立一个完整的连接**/
        client_sock = accept(server_sock,
                (struct sockaddr *)&client_name,
                &client_name_len);
        if (client_sock == -1)
            error_die("accept");
        /* accept_request(client_sock); */
        if (pthread_create(&newthread , NULL, (void *)accept_request, (void *)&client_sock) != 0)  // 创建线程
            perror("pthread_create");
    }

    /**释放套接字**/
    close(server_sock);

    return(0);
}
