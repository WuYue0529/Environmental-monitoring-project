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

using namespace std;
#define led_dev "/dev/led"

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
    while (1)
    {
        green_led_off();
        sleep(1);
        green_led_open();
        sleep(1);
    }
    return 0;
}

int alarm(void)
{
    while (1)
    {
        red_led_off();
        usleep(300000);
        red_led_open();
        usleep(300000);
    }
    return 0;
}