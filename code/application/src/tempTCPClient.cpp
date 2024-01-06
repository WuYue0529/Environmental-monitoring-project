#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

void getTempSocket()
{
    int sockfd, port = 6973;
    struct sockaddr_in serv_addr;
    socklen_t serv_addr_len;
    char buffer[256];
    int temperature;
    float rst;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }
    
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(port);
    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error on connecting");
        exit(1);
    }
    
    while (1) {
        // 接收服务器发送的温度数据
        serv_addr_len = sizeof(serv_addr);
        recv(sockfd, &temperature, sizeof(int), 0);

        // 打印温度
        // printf("tcpclient recv temperature: %d\n", temperature);
        // rst = (temperature >> 24) + ((temperature << 8) >> 24)/10.0;
        // printf("rst temperature: %f\n", rst);
        // printf("rst temperature: %f\n", rst);

        fflush(0);

        // 等待一秒钟
        sleep(1);
    }
    
    close(sockfd);
}

