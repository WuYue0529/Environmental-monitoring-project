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


int main()
{
	pthread_t tid;

	hello();
	// 自检

    // 初始化
	system("rm -f /home/w25q64/temp_log.txt");
	system("touch /home/w25q64/temp_log.txt");
	system("chmod +w /home/w25q64/temp_log.txt");

	// 添加任务 
	pthread_create(&tid, NULL, monitorTemperature, NULL);
	// idle
    while (1) {
        sleep(1); // 主线程延迟1秒
    }
    return 0;
}
