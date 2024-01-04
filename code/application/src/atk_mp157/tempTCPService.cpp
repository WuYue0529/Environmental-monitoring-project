/**
 * 将接收到的M4发送的温度值通过socket广播出去
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "bsp_mp157.h"

extern int envtemp;

int sendTemp()
{
    int sockfd, newsockfd, port = 6973;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[256];
    int temperature;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }
    
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error on binding");
        exit(1);
    }
    
    printf("Wait Connected to client\n");

    while (1) {
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");
            exit(1);
        }
        
        printf("Connected to client\
");
        fflush(0); 
        while (1) {
            // 获取当前的温度
            temperature = ((envtemp%10) << 8) | (envtemp/10);

            // 将温度转换为字节数组
            void *data = malloc(sizeof(int));
            memcpy(data, &temperature, sizeof(int));

            // 将字节数组发送给客户端
            send(newsockfd, data, sizeof(int), 0);

            // 等待1秒钟
            sleep(1);

            free(data);
        }
    }
    
    close(sockfd);
    close(newsockfd);
    return 0;
}