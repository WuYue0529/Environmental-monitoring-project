/**
 * 从虚拟串口获取M4发过来的温度数据
*/
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "bsp_mp157.h"

using namespace std;

extern int envtemp;
int BspGetTempM4()
{
    int fd = open("/dev/ttyRPMSG0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        cerr << "Error opening /dev/ttyRPMSG0" << endl;
        return 1;
    }

    // 第一次读出的数据直接丢掉
    char buffer[1024];
    read(fd, buffer, sizeof(buffer));

    while (true) {
        // 读取串口设备的输出，直到遇到换行符
        char buffer[1024];
        int bytes_read = 0;
        while (buffer[bytes_read - 1] != '\n') {
            bytes_read += read(fd, buffer + bytes_read, sizeof(buffer) - bytes_read);
        }

        // 将读取到的数据转换成整数
        int value = 0;
        for (int i = 0; i < bytes_read - 1; i++) {
            value = value * 10 + (buffer[i] - '0');
        }

        // 输出整数
        cout << value << endl;
        envtemp = value;
    }

    close(fd);
    return 0;
}