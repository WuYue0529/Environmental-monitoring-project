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
#include <pthread.h>
#include <at24c08.h>

using namespace std;
#define led_dev "/dev/led"


int temperature = 0;
unsigned char overtemperature_times = 0;
// 定义一个工作队列
struct work_queue {
    // 队列中的任务数量
    int count = 0;
    // 队列中的最大任务数量
    int max_count = 10;
    // 队列中的任务数组
    void (*tasks[10])();
};
// 创建一个工作队列
struct work_queue my_queue;
void alarm_hook(struct work_queue *queue);


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

static bool flag = true;
int run_normal(void)
{
    flag = true;
    green_led_open();
    delay(1);
    green_led_off();
    delay(1);
}

void *recode_handler(void *arg)
{
    for (int i = 0; i < my_queue.count; i++)
    {
        my_queue.tasks[i]();
    }
}

int alarm(void)
{
    pthread_t tid;
    if(flag)
    {
        overtemperature_times++;
        flag = false; 
        printf("alarm_hook \n");
        alarm_hook(&my_queue);
        pthread_create(&tid, NULL, recode_handler, NULL);
    }
    red_led_open();
    delay(0.3);
    red_led_off();
    delay(0.3);
}

void controlLED() {
    if (temperature > 26) {
        alarm();
    } else {
       run_normal();
    }
}

void *monitorTemperature(void *arg)
{
    while (1) {
        // 模拟获取环境温度
        temperature = BspGetTemp();

        // 控制LED状态
        controlLED();

        usleep(1);
    }
}

void recode_overtemperature_times()
{
    unsigned char readbuff = 0;
	BspWriteEeprom(0x33, 1, &overtemperature_times);
	BspReadEeprom(0x33, 1, &readbuff);
	printf("overtemperature_times : %d\n", readbuff);
    sleep(100);
}

// 钩子函数，将写eeprom的任务添加到工作队列中
void alarm_hook(struct work_queue *queue)
{
    if (queue->count < queue->max_count) {
        // 将写eeprom的任务添加到队列中
        queue->tasks[queue->count++] = recode_overtemperature_times;
    } else {
        printf("工作队列已满，无法添加新的任务！\n");
    }
}
