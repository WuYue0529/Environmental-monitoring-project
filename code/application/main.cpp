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
