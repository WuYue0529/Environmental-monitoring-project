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
	// gpio_direction_output(w25q64.cspin,1);

	/*杂项设备注册*/
    ret = misc_register(&w25q64_miscdev);

	w25q64.spi = spi;
	/*初始化spi_device */
	spi->mode = SPI_MODE_1;	/*MODE0，CPOL=0，CPHA=0*/
	spi_setup(spi);

	w25qxx_get_id(&ID);
	printk("This device_id = %04x\n",ID);

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

