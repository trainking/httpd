#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include "include/common.h"


/*
* 从socket中读取http报文的一行
* @param int sock socket描述符
* @param char *buf 读取结果缓存区
* @param int size buf的空间大小
* @return int 读取的内容长度
*/
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

u_int32_t ipv4_to_int(char *ip)
{
    char str_ip_index[4]={'\0'};
	unsigned int ip_int,ip_add=0;
    int j =0,a=3;
    for(unsigned int i =0;i<=strlen(ip);i++) 
    {
        if (ip[i]=='\0'||ip[i]=='.')
		{			
			ip_int =atoi(str_ip_index);	
			if (ip_int>255)
			{
				printf("IP地址有误\n");
				system("pause");
				return 0;				
			}
 
			ip_add+=(ip_int*((unsigned int)pow(256.0,a)));			
			a--;
			memset(str_ip_index,0,sizeof(str_ip_index));
			j=0;
			continue;
		}
        str_ip_index[j]=ip[i];
        j++;
    }
    return (u_int32_t)ip_add;
}

// html_path 返回路径地址
char* html_path() {
    return "html";
}