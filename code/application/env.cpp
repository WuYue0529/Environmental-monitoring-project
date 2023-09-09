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
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

int main()
{
    int fd, ret;
    unsigned char result[2];
    int TH, TL;
    short tmp = 0;
    float temperature;
    int flag = 0;

    fd = open("/dev/ds18b20", 0);

    if(fd < 0)
    {
        perror("open device failed\n");
        exit(1);
    }
    else
        printf("Open success!\n");

    while(1)
    {
        ret = read(fd, &result, sizeof(result)); 
		if(ret == 0) {	/* 读取到数据 */
			TL = result[0];
			TH = result[1];
    
			if((TH == 0XFF) && (TL == 0XFF))/* 如果读取到数据为0XFFFF就跳出本次循序 */
				continue;
			if(TH > 7) {	/* 负数处理 */
				TH = ~TH;
				TL = ~TL;
				flag = 1;	/* 标记为负数 */
			}

			tmp = TH;
			tmp <<= 8;
			tmp += TL;
        
			if(flag == 1) {
				temperature = (float)(tmp+1)*0.0625; /* 计算负数的温度 */
				temperature = -temperature;
			}else {
				temperature = (float)tmp *0.0625;	/* 计算正数的温度 */
			}            

			if(temperature < 125 && temperature > -55) {	/* 温度范围 */
				printf("Current Temperature: %f\n", temperature);
			}
		}
    flag = 0;
    sleep(1);
    }
	close(fd);	/* 关闭文件 */
}
