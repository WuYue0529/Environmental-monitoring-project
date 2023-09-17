

// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/ioctl.h>
// #include <linux/i2c.h>
// #include <linux/i2c-dev.h>
// #include <linux/types.h>

// #define i2c_dev "/dev/at24c08"

// int main(void)
// {
//         int fd, i = 0, count = 0;
//         fd = open(i2c_dev, O_RDWR);
//          /* 读取的数据:片内地址+1个数据 */
//         unsigned char rbuffer[2];
//         unsigned char address[5];
//         address[0] = 0x05;
//         address[1] = 0x06;
//         address[2] = 0x07;
//         address[3] = 0x08;
//         address[4] = 0x09;
//         unsigned char data[5] = {'i', 'l', '2', '2', '2'};
//         unsigned char wbuffer[3]; /* 片内地址+h */
//         if(fd < 0){
//                 printf("open %s failed\r\n", i2c_dev);
//                 exit(-1);
//         }else {
//                 printf("open %s succeed\n", i2c_dev);
//                 rbuffer[0] = address[0];
//                 read(fd, rbuffer, 1);
//                 printf("rbuffer[0] is [%c]-", rbuffer[0]);
//                 printf("rbuffer[1] is [%c]-\n", rbuffer[1]);

//                 wbuffer[0] = address[0];
//                 wbuffer[1] = data[0];
//                 wbuffer[2] = data[1];
//                 write(fd, wbuffer, 2);


//                 printf("check write result:\n");
//                 rbuffer[0] = address[0];
//                 read(fd, rbuffer, 1);
//                 printf("rbuffer[0] is [%c]-", rbuffer[0]);
//                 printf("rbuffer[1] is [%c]-", rbuffer[1]);
//                 // printf("#####################################\n");
//                 // for(i = 0; i < 5; i++)
//                 // {
//                 //         rbuffer = address[i];
//                 //         read(fd, &rbuffer, 2);
//                 //         printf("-0x[%c]-", rbuffer);
//                 //         printf("-0x[%c]-",*( &rbuffer+1));

//                 // }
//                 // printf("#####################################\n");
//                 // for(i = 0; i < 5; i++)
//                 // {
//                 //         wbuffer[0] = address[i];
//                 //         wbuffer[1] = data[i];
//                 //         write(fd, wbuffer, 2);
//                 // }
//                 // printf("#####################################\n");
//                 // for(i = 0; i < 5; i++)
//                 // {
//                 //         rbuffer = address[i];
//                 //         read(fd, &rbuffer, 1);
//                 //         printf("-0x[%c]-", rbuffer);
//                 // }
//                 printf("\n");
//         }
//         close(fd);
//         return 0;
// }






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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/types.h>

#define i2c_dev "/dev/at24c08"

int main()
{
    int fd, ret;
    unsigned char result[2];
    int TH, TL;
    short tmp = 0;
    float temperature;
    int flag = 0;
	int fd1, i = 0, count = 0;

    unsigned char rbuffer[2];
	unsigned char address[5];
	        address[0] = 0x05;
        address[1] = 0x06;
        address[2] = 0x07;
        address[3] = 0x08;
        address[4] = 0x09;
	// unsigned char data[5];
	unsigned char wbuffer[3]; /* 片内地址+h */
unsigned char data[5] = {'y', 'u', '2', '2', '2'};
    // fd = open("/dev/ds18b20", 0);
    fd1 = open(i2c_dev, O_RDWR);
printf("fd1 = %d\n", fd1);
	// rbuffer[0] = address[0];
    // read(fd1, rbuffer, 1);
	// printf("rbuffer[0] is [%c]\n", rbuffer[0]);
	// printf("rbuffer[1] is [%c]\n", rbuffer[1]);

	wbuffer[0] = address[0];
                wbuffer[1] = 'd';
                wbuffer[2] = '3';
	printf("11\n");
	write(fd1, wbuffer, 2);
	printf("11\n");

	rbuffer[0] = address[0];
    read(fd1, rbuffer, 1);
	printf("rbuffer[0] is [%c]\n", rbuffer[0]);
	printf("rbuffer[1] is [%c]\n", rbuffer[1]);



    // if(fd < 0)
    // {
    //     perror("open device failed\n");
    //     exit(1);
    // }
    // else
    //     printf("Open success!\n");



    // while(1)
    // {
    //     ret = read(fd, &result, sizeof(result)); 
	// 	if(ret == 0) {	/* 读取到数据 */
	// 		TL = result[0];
	// 		TH = result[1];
    
	// 		if((TH == 0XFF) && (TL == 0XFF))/* 如果读取到数据为0XFFFF就跳出本次循序 */
	// 			continue;
	// 		if(TH > 7) {	/* 负数处理 */
	// 			TH = ~TH;
	// 			TL = ~TL;
	// 			flag = 1;	/* 标记为负数 */
	// 		}

	// 		tmp = TH;
	// 		tmp <<= 8;
	// 		tmp += TL;
        
	// 		if(flag == 1) {
	// 			temperature = (float)(tmp+1)*0.0625; /* 计算负数的温度 */
	// 			temperature = -temperature;
	// 		}else {
	// 			temperature = (float)tmp *0.0625;	/* 计算正数的温度 */
	// 		}            

	// 		if(temperature < 125 && temperature > -55) {	/* 温度范围 */
	// 			printf("Current Temperature: %f\n", temperature);
	// 		}
	// 	}
    // flag = 0;
    // sleep(1);
    // }
	close(fd1);	/* 关闭文件 */
    return 0;

}
