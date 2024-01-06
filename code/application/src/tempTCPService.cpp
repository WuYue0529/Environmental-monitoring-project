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
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <fcntl.h>


int envtemp = 0;
void sendTemp()
{
    int sockfd, newsockfd, port = 6973;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[256];
    int temperature;
    int i = 0;

    std::ofstream file("/home/w25q64/temp.rst", std::ios::out);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
    }

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

    while (1)
    {
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
        while (1)
        {
            // 获取当前的温度
            {
                // std::lock_guard<std::mutex> lock(mtx);
                // printf("envtemp: %d\n", envtemp);
                // temperature = ((envtemp%10) << 8) | (envtemp/10);
                temperature = envtemp;

                // printf("service send temperature: %d\n", temperature);
            }
            // 将温度转换为字节数组
            void *data = malloc(sizeof(int));
            memcpy(data, &temperature, sizeof(int));

            // 将字节数组发送给客户端
            send(newsockfd, data, sizeof(int), 0);
            file.open("/home/w25q64/temp.rst", std::ios::out);
            file << temperature << std::endl;
            file.flush();
            file.close();

            // 等待1秒钟
            sleep(1);
            free(data);
        }
        // 添加延时
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    close(sockfd);
    close(newsockfd);
    file.close();

}