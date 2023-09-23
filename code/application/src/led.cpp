#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/types.h>
#include <string>
#include <ds18b20.h>
#include <common.h>



using namespace std;
#define led_dev "/dev/led"

extern bool ALARM_FLAG;
int temperature = 0;

int open_device(string device)
{
    int fd = -1;
    fd = open(device.c_str(), O_RDWR);
    if(fd < 0)
    {
        printf("open %s failed\r\n", led_dev);
        exit(-1);
    }
    return fd;
}

int red_led_off(void)
{
    int fd = -1;
    int retvalue;
    unsigned char cmd[2] = {0};

    fd = open_device(led_dev);

    cmd[0] = 0;
    cmd[1] = 0;
    retvalue = write(fd, cmd, sizeof(cmd));
    close(fd);
    return 0;
}

int red_led_open(void)
{
    int fd = -1;
    int retvalue;
    unsigned char cmd[2] = {0};

    fd = open_device(led_dev);

    cmd[0] = 0;
    cmd[1] = 1;
    retvalue = write(fd, cmd, sizeof(cmd));
    close(fd);
    return 0;
}

int green_led_off(void)
{
    int fd = -1;
    int retvalue;
    unsigned char cmd[2] = {0};

    fd = open_device(led_dev);

    cmd[0] = 1;
    cmd[1] = 0;
    retvalue = write(fd, cmd, sizeof(cmd));
    close(fd);
    return 0;
}

int green_led_open(void)
{
    int fd = -1;
    int retvalue;
    unsigned char cmd[2] = {0};

    fd = open_device(led_dev);

    cmd[0] = 1;
    cmd[1] = 1;
    retvalue = write(fd, cmd, sizeof(cmd));
    close(fd);
    return 0;
}

int run_normal(void)
{
    green_led_open();
    delay(1);
    green_led_off();
    delay(1);
}

int alarm(void)
{
    red_led_open();
    delay(0.3);
    red_led_off();
    delay(0.3);
}

void controlLED() {
    if (temperature > 25) {
        alarm();
    } else {
       run_normal();
    }
}

void *monitorTemperature(void *arg) {
    while (1) {
        // 模拟获取环境温度
        temperature = BspGetTemp();

        // 控制LED状态
        controlLED();

        usleep(1);
    }
}
