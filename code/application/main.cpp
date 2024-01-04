/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: ds18b20App.c
作者	  	: 正点原子Linux团队
版本	   	: V1.0
描述	   	: ds18b20设备测试APP。
其他	   	: 无
使用方法	 ：./ds18b20App /dev/ds18b20
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2021/08/25 正点原子Linux团队创建
***************************************************************/
#include <iostream>
#include "common.h"
#include "unistd.h"
#include <pthread.h>
// #include "test.h"
#include <linux/input.h>
#include <chrono>
#include <thread>
#include <termios.h>
#include "bsp_mp157.h"

int main()
{
	pthread_t tid_gettemp;
	pthread_t tid_sendtemp;
	pthread_t tid_mqtt;
	pthread_t tid_alarm;
	pthread_t tid_qt;


	hello();

	// 自检

	pthread_create(&tid_mqtt, NULL, BspGetTempM4, NULL);
    std::thread myThread(BspGetTempM4);  // 创建线程并指定线程函数
    myThread.detach();  // 分离线程，使其在后台运行

	// idle
    while (1) {
        sleep(60); // 主线程延迟1秒
        std::cout << "mp157 running..." << std::endl;
        fflush(0);
    }
    return 0;
}
