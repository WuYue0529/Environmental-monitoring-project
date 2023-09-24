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
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
#define led_dev "/dev/led"


float temperature = 0;
unsigned char overtemperature_times = 0;
std::string monitor_log;

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
void run_normal(void)
{
    flag = true;
    green_led_open();
    delay(1);
    green_led_off();
    delay(1);
}

void *recode_handler(void *arg)
{
    unsigned char readbuff = 0;
	BspWriteEeprom(0x33, 1, &overtemperature_times);
	BspReadEeprom(0x33, 1, &readbuff);
	printf("overtemperature_times : %d\n", readbuff);
}

static pid_t pid;
std::stringstream ss;
void *log_handler(void *arg)
{
    std::system("date > temp.txt");
    std::ifstream file("temp.txt");
    string date;
    if (file.is_open()) 
    {
        std::getline(file, date);
        file.close();
        std::remove("temp.txt");
    }
    std::stringstream ss;
    ss << date << endl << "No : " << (int)overtemperature_times << " & temperature is : " << temperature;
    monitor_log = ss.str();
    std::cout << monitor_log << std::endl;
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(-1);
    }
    
    if (pid == 0) // 子进程
    {
        // 向flash写log
        // ss << monitor_log;
        // 打开文件std::ios::app 表示以追加模式打开文件
        std::ofstream file("/home/w25q64/temp_log.txt", std::ios::app);
        // 将 ss 的内容写入文件并换行
        file << ss.str() << std::endl;
        file.close();
        system("sync");
        exit(0);
    } 
    else
    {
        // do nothing
    }
}

void alarm(void)
{
    pthread_t tid;
    pthread_t tid_log;
    if(flag)
    {
        overtemperature_times++;
        flag = false;
        pthread_create(&tid, NULL, recode_handler, NULL);
        pthread_create(&tid_log, NULL, log_handler, NULL);
    }
    red_led_open();
    delay(0.3);
    red_led_off();
    delay(0.3);
}

void controlLED()
{
    if (temperature > 26.0) {
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

