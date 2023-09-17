// SPDX-License-Identifier: GPL-3.0-only
/*
 * W25Q64 driver for mp157(SPI)
 *
 * 2023-09-14
 * WuYue
 */
#include <linux/types.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include "interface.h"

/* w25q64设备结构体 */
struct w25q64_dev {
 	int cspin; 					/*片选脚*/
	struct spi_device *spi;		/* spi设备 */
 	w25qxx_data_def data;
};
struct w25q64_dev w25q64;

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/*
函数功能：指定位置读取指定长度的数据
参    数：
0x03 表示读取数据的指令。
*/
void  W25Q64_ReadData(unsigned int addr,void *p,unsigned int len)
{	
	/*使用硬件SPI同步读写时序*/
	char tx_buf[4];
	tx_buf[0]=0x03;       //读指令
	tx_buf[1]=addr>>16;   //以下是地址指令
	tx_buf[2]=addr>>8;
	tx_buf[3]=addr;
	spi_write(w25q64.spi, tx_buf, 4);
	spi_read(w25q64.spi, p, len);
}

/*
函数功能：写使能
*/
void  W25Q64_WriteEnabled(void)
{
	/*使用硬件SPI同步读写时序*/
	char tx_buf[1]={0x06};
	struct spi_message m;
	struct spi_transfer t=
	{
		.tx_buf=tx_buf,
		.len=1,
		.delay_usecs=0,
		.speed_hz=1000000,
		.bits_per_word=8
	};
	spi_message_init(&m);
	spi_message_add_tail(&t,&m);
	spi_sync(w25q64.spi,&m);
}

/*
函数功能：读状态
*/
void W25Q64_GetBusyStat(void)
{
	unsigned char stat=1;
	/*使用硬件SPI同步读写时序*/
	char tx_buf[2]={0x05,0xFF};
	char rx_buf[2];
	while(stat&0x01)  //判断状态最低位
	{	
		struct spi_message m;
		struct spi_transfer t=
		{
			.tx_buf=tx_buf,
			.rx_buf=rx_buf,
			.len=2,
			.delay_usecs=0,
			.speed_hz=1000000,
			.bits_per_word=8
		};
		spi_message_init(&m);
		spi_message_add_tail(&t,&m);
		spi_sync(w25q64.spi,&m);
		stat=rx_buf[1]; //得到状态寄存器
	}
}

/*
函数功能：扇区擦除
参    数：
		unsigned int addr：扇区的地址
说   明：一个扇区是4096字节，擦除一个扇区时间至少150ms
*/
void W25Q64_ClearSector(unsigned int addr)
{
	W25Q64_WriteEnabled(); //写使能
	W25Q64_GetBusyStat();  //检测状态寄存器
	
	/*使用硬件SPI同步读写时序*/
	unsigned char tx_buf[4];
	tx_buf[0]=0x20;
	tx_buf[1]=addr>>16;
	tx_buf[2]=addr>>8;
	tx_buf[3]=addr;
	
	char rx_buf[4];
	struct spi_message m;
	struct spi_transfer t=
	{
		.tx_buf=tx_buf,
		.rx_buf=rx_buf,
		.len=4,
		.delay_usecs=0,
		.speed_hz=1000000,
		.bits_per_word=8
	};
	spi_message_init(&m);
	spi_message_add_tail(&t,&m);
	spi_sync(w25q64.spi,&m);
	W25Q64_GetBusyStat();  //检测状态寄存器
}

 
/*
函数功能：页编程
参    数：
		unsigned int addr：写入的地址
        void  *p：将要写入的数据
		unsigned int  len：写入的长度
说    明：每次最多只能写入256字节
*/
void W25Q64_PageWrite(unsigned int addr,void*p,unsigned int len)
{
	 unsigned short i;
	 unsigned char *buff=p;
	 W25Q64_WriteEnabled(); //写使能
	 
	/*使用硬件SPI同步读写时序*/
	unsigned char tx_buf[4];
	tx_buf[0]=0x02;       //页写指令
	tx_buf[1]=(addr>>16)&0xFF;   //以下是地址指令
	tx_buf[2]=(addr>>8)&0xFF;
	tx_buf[3]=(addr&0xFF);
	
	//写数据
	spi_write(w25q64.spi,tx_buf,4);
	//写数据
	spi_write(w25q64.spi,p,len);
	W25Q64_GetBusyStat();  //检测状态寄存器
}


/*
函数功能：在任意地址写入任意数据，不进行校验
参    数：
	unsigned int addr：写入数据的地址
	void *p ：写入的数据
	unsigned int len ：写入数据的长度
*/
void W25Q64_WriteDataONCheck(unsigned int addr,void *p,unsigned int len)
{
	unsigned char *buff=p;
	unsigned short page_remain=256-addr%256;  //当前地址开始一页剩下的空间
	unsigned short remain_len;      //剩余未写入的长度
	if(len<page_remain)  //当前这一页剩下的空间足够可以写入
	{ 
		page_remain=len;
	}

	while(1)
	{
		W25Q64_PageWrite(addr,buff,page_remain); 
		if(page_remain==len)break;
		addr+=page_remain; //地址向后移动
		buff+=page_remain; //地址向后移动
		len-=page_remain;  //长度递减

		if(len>256)page_remain=256;
		else page_remain=len;
	} 
}

/*
函数功能：在任意地址写入任意数据，对扇区进行校验
参    数：
	unsigned int addr：写入数据的地址
	void *p ：写入的数据
	unsigned int len ：写入数据的长度
说明：一个扇区的空间4096字节
*/
static unsigned char W25Q64_BUFF[1024*4]; //用来检验一个扇区的数据是否需要擦除
void W25Q64_WriteData(unsigned int addr,void *p,unsigned int len)
{
	unsigned int sector_len=4096-addr%4096;  //剩余空间大小 
	unsigned char *buff=p;
	unsigned int i=0;
	if(len<sector_len)  //剩下的空间足够写
	{
		sector_len=len;
	}
	while(1)
	{
	  W25Q64_ReadData(addr,W25Q64_BUFF,sector_len);
	  for(i=0;i<sector_len;i++)
	  {
		   if(W25Q64_BUFF[i]!=0xFF)
			{
			   W25Q64_ClearSector(addr); //擦除扇区
			   break;
			}
	  }
		W25Q64_WriteDataONCheck(addr,buff,sector_len);
		if(sector_len==len)break; //数据写完

		buff+=sector_len;
		addr+=sector_len;
		len-=sector_len;

		if(len>4096)
		{
			sector_len=4096;
		}
		else
		{
			sector_len=len;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////




/* 设备操作函数 */
static const struct file_operations w25q64_fops = {
	.owner = THIS_MODULE,
	// .open = w25q64_open,
	// .read = w25q64_read,
	// .write = w25q64_write,
	// .release=w25q64_release
};

static int w25qxx_get_id(unsigned int *ID)
{
	struct spi_transfer xfer[2];
	struct spi_message msg;
	unsigned char tx_buf[4] = {0x90, 0, 0, 0};
	unsigned char rx_buf[2];

	memset(&xfer, 0, sizeof(xfer));

	xfer[0].tx_buf = tx_buf;
	xfer[0].len = 4;

	xfer[1].rx_buf = rx_buf;
	xfer[1].len = 2;

	spi_message_init(&msg);
	spi_message_add_tail(&xfer[0], &msg);
	spi_message_add_tail(&xfer[1], &msg);

	spi_sync(w25q64.spi, &msg);
	// printk("Receive success : %x, %x", rx_buf[0], rx_buf[1]);
	*ID = (rx_buf[0]<<8) | rx_buf[1];
	return 0;
}

/* MISC设备结构体 */
static struct miscdevice w25q64_miscdev = {
	.minor = MISCFlash_MINOR,
	.name = MISCFlash_NAME,
	.fops = &w25q64_fops,
};

static int w25q64_probe(struct spi_device *spi)
{
	int ret=0;
	unsigned int ID;
	unsigned char read_data[5] = {'i', 'i', 'i', 'i'};
	unsigned char write_data[5] = {'a', 'b', 'c', 'd'};

	// struct device_node *spi_node = spi->dev.of_node;

	printk("w25q64 driver probe ok\r\n");
    // struct device *dev = &spi -> cs_gpio;
    // int ret;

    w25q64.cspin = spi -> cs_gpio;
	if(!gpio_is_valid(w25q64.cspin))
    {
        printk("failed to get gpio\n\r");
        return -EINVAL;
    }
	// w25q64.cspin = of_get_named_gpio(spi_node, "cs-gpios",0);
	printk("gpio = %d\n",w25q64.cspin);
   	ret = gpio_request(w25q64.cspin,"spi-cs");
	gpio_direction_output(w25q64.cspin, 1); // 实际并没有生效，gpio获取的错了？

	/*杂项设备注册*/
    ret = misc_register(&w25q64_miscdev);

	w25q64.spi = spi;
	/*初始化spi_device */
	spi->mode = SPI_MODE_1;	/*MODE0，CPOL=0，CPHA=0*/
	spi_setup(spi);

	w25qxx_get_id(&ID);
	printk("This device_id = %04x\n",ID);

	W25Q64_WriteDataONCheck(0x50, write_data, 4);
	W25Q64_ReadData(0x50, read_data, 4);
	printk("W25Q64_ReadData = %c %c %c %c\n",read_data[0], read_data[1], read_data[2], read_data[3]);

	return ret;
}

static int w25q64_remove(struct spi_device *spi)
{
	/* 注销misc设备 */
	misc_deregister(&w25q64_miscdev);
   	gpio_free(w25q64.cspin);

	return 0;
}

// /* 传统匹配方式ID列表 */
// static const struct spi_device_id w25q64_id[] = {
// 	{"mp157led", 0},
// 	{}
// };

/* 设备树匹配列表 */
static const struct of_device_id w25q64_of_spi_match[] = {
	{ .compatible = "winbond,w25q64" },
	{ /* Sentinel */ }
};

static struct spi_driver w25q64_spi_driver = {
	.probe = w25q64_probe,
	.remove = w25q64_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "winbond",
		.of_match_table = w25q64_of_spi_match,
	},
	// .id_table = w25q64_id,
};

module_spi_driver(w25q64_spi_driver);

MODULE_DESCRIPTION("W25Q64 SPI bus driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charles");
MODULE_INFO(intree, "Y");
