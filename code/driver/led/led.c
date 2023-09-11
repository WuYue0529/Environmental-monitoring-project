
// SPDX-License-Identifier: GPL-3.0-only
/*
* led driver for mp157
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
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/errno.h>
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
#include <linux/irq.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/platform_device.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define GPIOLED_CNT			1		  	/* 设备号个数 */
#define GPIOLED_NAME		"led"	/* 名字 */
#define LEDOFF 				0			/* 关灯 */
#define LEDON 				1			/* 开灯 */

struct led_device {
	dev_t devid;				/* 设备号	*/
	struct cdev cdev;			/* cdev		*/
	struct class *class;		/* 类 		*/
	struct device *device;		/* 设备		*/	
	struct device_node *node;	/* LED设备节点 */
	int gpio_led[2];				/* LED灯GPIO标号 */
};
struct led_device mp157led;

static int led_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	int retvalue;
	unsigned char user_data[2];

	retvalue = copy_from_user(user_data, buf, cnt);
	if(retvalue < 0) {
		printk("kernel write failed!\r\n");
		return -EFAULT;
	}
	// led_switch(user_data[0], user_data[1]);

	if(user_data[1] == LEDON)
	{
		gpio_set_value(mp157led.gpio_led[user_data[0]], 0);
	}
	else if(user_data[1] == LEDOFF)
	{
		gpio_set_value(mp157led.gpio_led[user_data[0]], 1);
	}
	else
	{
		printk("Get error cmd\r\n");
	}

	return 0;
}


static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .write = led_write,
};

static int led_probe(struct platform_device *pdev)
{
    int ret;
    struct device *leddev = &pdev->dev;
    struct device_node *childled_node = NULL;
    struct device_node *leds_node = pdev->dev.of_node;

    dev_info(&pdev->dev, " led device matched success");

    childled_node = of_get_next_child(leds_node, childled_node);
    mp157led.gpio_led[0] = of_get_gpio(childled_node, 0);
    printk("led0 gpio : %d \r\n", mp157led.gpio_led[0]);

    childled_node = of_get_next_child(leds_node, childled_node);
    mp157led.gpio_led[1] = of_get_gpio(childled_node, 0);
    printk("led1 gpio : %d \r\n", mp157led.gpio_led[1]);

    ret = gpio_request_one(mp157led.gpio_led[0], GPIOF_OUT_INIT_LOW, "leds");
    ret = gpio_request_one(mp157led.gpio_led[1], GPIOF_OUT_INIT_LOW, "leds");
    gpio_direction_output(mp157led.gpio_led[0], 0);
    gpio_direction_output(mp157led.gpio_led[1], 0);


    alloc_chrdev_region(&mp157led.devid, 0, GPIOLED_CNT, GPIOLED_NAME);
    mp157led.cdev.owner = THIS_MODULE;
    cdev_init(&mp157led.cdev, &led_fops);
    cdev_add(&mp157led.cdev, mp157led.devid, GPIOLED_CNT);
    mp157led.class = class_create(THIS_MODULE, GPIOLED_NAME);
    mp157led.device = device_create(mp157led.class, NULL, mp157led.devid, NULL, GPIOLED_NAME);
    printk("led driver probe success!\r\n");

    return 0;
}

static int led_remove(struct platform_device *pdev)
{
    gpio_set_value(mp157led.gpio_led[0], 1); 	/* 卸载驱动的时候关闭LED */
    gpio_set_value(mp157led.gpio_led[1], 1); 	/* 卸载驱动的时候关闭LED */
	gpio_free(mp157led.gpio_led[0]);	/* 注销GPIO */
	gpio_free(mp157led.gpio_led[1]);	/* 注销GPIO */

	cdev_del(&mp157led.cdev);				/*  删除cdev */
	unregister_chrdev_region(mp157led.devid, GPIOLED_CNT); /* 注销设备号 */
	device_destroy(mp157led.class, mp157led.devid);	/* 注销设备 */
	class_destroy(mp157led.class); /* 注销类 */

    return -EPERM;
}


static const struct of_device_id led_of_match[] = {
    {.compatible = "mp157led"},
    {}
};

static struct platform_driver led_driver = {
    .driver = {
		.name = "mp157led",
        .of_match_table	= led_of_match,
	},
	.probe = led_probe,
    .remove = led_remove,
};


module_platform_driver(led_driver);

MODULE_DESCRIPTION("LED Driver, buy form zave");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("WuYue");
MODULE_INFO(intree, "Y");

