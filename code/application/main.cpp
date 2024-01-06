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

void qt_thread();

int main()
{
	hello();

	// 写资产信息
	burnBoardInfoToEeprom();

	//开启状态灯
	std::thread AlarmThread(BspAlarm);
	AlarmThread.detach();

	std::thread tempThread(BspGetTempM4);
	tempThread.detach();

	std::cout << "begin sendTempThread" << std::endl;
	std::thread sendTempThread(sendTemp);
	sendTempThread.detach();
	
    sleep(2); // 主线程延迟1秒
	std::cout << "begin getTempThread" << std::endl;
	std::thread getTempThread(getTempSocket);
	getTempThread.detach();

	// 开启qt程序
	std::thread qtThread(qt_thread);
	qtThread.detach();

	//开启mqtt上报
	std::thread mqttThread(mqtt_thread);
	mqttThread.detach();

	// idle
    while (1) {
        sleep(60); // 主线程延迟1秒
        std::cout << "mp157 running..." << std::endl;
        fflush(0);
    }
    return 0;
}

void qt_thread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 5));
    std::cout << "begin qt" << std::endl;

	system("/qt/tt.sh");
}