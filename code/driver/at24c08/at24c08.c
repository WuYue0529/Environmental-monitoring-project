
// SPDX-License-Identifier: GPL-3.0-only
/*
 * at24c08 driver for mp157(I2C)
 *
 * 2023-09-09
 * WuYue
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/i2c.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/device.h>

 /* i2c设备地址 */
static unsigned short addr = 0x50;

#define AT24C08_CNT	1
#define AT24C08_NAME	"at24c08"

struct at24c08_data {
	struct i2c_client *at24c08_client;	/* i2c 设备 */
	dev_t devid;			/* 设备号 	 */
	struct cdev at24c08_cdev;		/* cdev 	*/
	struct class *at24c08_class;	/* 类 		*/
	struct device *at24c08_device;	/* 设备 	 */
	struct device_node	*at24c08_nd; /* 设备节点 */
};
struct at24c08_data zave;


static ssize_t at24c08_ops_write(struct file *filp, const char __user *buffer, size_t size, loff_t *off)
{
	struct i2c_msg msg[1];
	unsigned char *args = NULL;
	args = kzalloc(size, GFP_KERNEL);
	// if(size != 2){
	// 		return -EINVAL;
	// }

	printk("at24c08_write......\n");
	/* 成功返回0;失败返回未完成字节数 */
	copy_from_user(args, buffer, size + 1);
	// printk("write parameters : args[0] = 0x%x, args[1] = 0x%x\n", args[0], args[1]);

	/* args[0]:addr, args[1]:value */
	msg[0].addr = addr;    /* 设备地址 */
	msg[0].buf = args;     /* 写入的数据: 寄存器 + 数据 */
	msg[0].len = size + 1;        /* 长度 */
	msg[0].flags = 0;      /* 写标志 */

	if(i2c_transfer(zave.at24c08_client->adapter, msg, 1) == 1){
			printk("at24c08_write succeed\n");
			return 2;
	}
	else{
			printk("at24c08_write failed\n");
			return -EIO;
	}
}

static ssize_t at24c08_ops_read(struct file *filp, char __user *buffer, size_t size, loff_t *off)
{
	struct i2c_msg msg[2];    /* 封装消息 */
	unsigned char args;       /* 用户态传进来的要读取的从机寄存器 */
	unsigned char *data = NULL;       /* 要返回的数据 */

	data = kzalloc(size, GFP_KERNEL);

	printk("at24c08_read begin\n");
	copy_from_user(&args, buffer, 1);

	/* at24c08随机读操作 先写地址,再读取数据 一共2次通讯 */
	msg[0].addr = addr;   /* 从设备地址 0x50  */
	msg[0].buf = &args;   /* 要读取的地址 */
	msg[0].len = 1;       /* 消息的长度 */
	msg[0].flags = 0;     /* 写 */

	/* 再读 */
	msg[1].addr = addr;
	msg[1].buf = data;        /* 接收读取的数据 */
	msg[1].len = size;            /* 要读取的数据长度 */
	msg[1].flags = I2C_M_RD;   /* 读 */

	/* 与目标设备进行2次通讯 */
	if(i2c_transfer(zave.at24c08_client->adapter, msg, 2) == 2) {
			copy_to_user(buffer, data, size);
			kfree(data);
			return 1;
	}
	else{
			printk("at24c08_read failed\n");
			kfree(data);
			return -EIO;
	}
}


int at24c08_ops_open(struct inode *inode, struct file *filp)
{
        printk("at24c08 open\n");
        return 0;
}

static int at24c08_ops_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/* at24c08_ops操作函数 */
static const struct file_operations at24c08_ops = {
	.owner = THIS_MODULE,
	.open = at24c08_ops_open,
	.read = at24c08_ops_read,
	.write = at24c08_ops_write,
	.release = at24c08_ops_release,
};

static int at24c08_probe(struct i2c_client *client,
			const struct i2c_device_id *i2c_device)
{
	dev_info(&client->dev, "at24c08 device matched success");
	alloc_chrdev_region(&zave.devid, 0, AT24C08_CNT, AT24C08_NAME);
	zave.at24c08_cdev.owner = THIS_MODULE;
	cdev_init(&zave.at24c08_cdev, &at24c08_ops);
	cdev_add(&zave.at24c08_cdev, zave.devid, AT24C08_CNT);

	zave.at24c08_class = class_create(THIS_MODULE, AT24C08_NAME);
	zave.at24c08_device = device_create(zave.at24c08_class, NULL, zave.devid, NULL, AT24C08_NAME);
	zave.at24c08_client = client;
	/* 保存ap3216cdev结构体 */
	i2c_set_clientdata(client, &zave);
	dev_info(&client->dev, "at24c08_dev made success");
	return 0;

}

static int at24c08_remove(struct i2c_client *client)
{
	cdev_del(&zave.at24c08_cdev);
	unregister_chrdev_region(zave.devid, AT24C08_CNT);
	device_destroy(zave.at24c08_class, zave.devid);
	class_destroy(zave.at24c08_class);
	return 0;
}

static const struct of_device_id at24c08_of_match[] = {
	{.compatible = "zave,at24c08"},
	{},
};

/* This is the driver that will be inserted */
static struct i2c_driver at24c08_driver = {
	.driver = {
		.name = "zave,at24c08",
		.owner = THIS_MODULE,
		.of_match_table = at24c08_of_match,
	},
	.probe = at24c08_probe,
	.remove = at24c08_remove,
	// .id_table 	= at24c08_id, /* id列表 */
};

module_i2c_driver(at24c08_driver);

MODULE_DESCRIPTION("at24c08 driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charles");
MODULE_INFO(intree, "Y");
