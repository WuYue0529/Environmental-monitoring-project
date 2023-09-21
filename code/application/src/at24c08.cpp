#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/types.h>
#include <string.h>


#define at24c08_dev "/dev/at24c08"

int BspReadEeprom(int reg, int len, unsigned char *buff);
int BspWriteEeprom(int reg, int len, unsigned char *buff);


int BspReadEeprom(int reg, int len, unsigned char *buff)
{
        int fd = -1;
        unsigned char *rbuffer = NULL;
        unsigned char address = reg;
        fd = open(at24c08_dev, O_RDWR);
        if(fd < 0)
        {
            printf("open %s failed\r\n", at24c08_dev);
            exit(-1);
        }

        rbuffer = (unsigned char *)malloc(len + 1);
        memset(rbuffer, 0, sizeof(rbuffer));
        rbuffer[0] = address;
        read(fd, rbuffer, len);

        memcpy(buff, rbuffer, len);

        close(fd);
        return 0;
}

int BspWriteEeprom(int reg, int len, unsigned char *buff)
{
        int fd = -1;
        unsigned char *wbuffer = NULL;
        unsigned char address = reg;
        fd = open(at24c08_dev, O_RDWR);
        if(fd < 0)
        {
            printf("open %s failed\r\n", at24c08_dev);
            exit(-1);
        }

        wbuffer = (unsigned char *)malloc(len + 3);
        memset(wbuffer, 0, sizeof(wbuffer));
        memcpy(wbuffer + 1, buff, len);

        wbuffer[0] = address;
        write(fd, wbuffer, len);

        close(fd);
        return 0;
}

