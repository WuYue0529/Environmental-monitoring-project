
// SPDX-License-Identifier: GPL-3.0-only
/*
* ds18b20 module driver for mp157
*
* 2023-09-04
* WuYue
*/
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/errno.h>

#define HIGH 1	
#define LOW 0

struct ds18b20_device {
    struct miscdevice   ds18b20_miscdev;
    int                 gpio;
    unsigned char       data[2];
    struct timer_list   timer;
    struct work_struct  work;
};
struct ds18b20_device ds18b20;

/*
 * @description	 : 	设置GPIO的输出模式
 * @param - value: 	输出value的值，高或低
 * @return 	     :  无
 */
static void set_gpio_output(int value)
{
    if(HIGH == value)
        gpio_direction_output(ds18b20.gpio, 1);
    else
        gpio_direction_output(ds18b20.gpio, 0);
}
/*
 * @description	: 	设置GPIO为输入模式
 * @param 		:	无
 * @return 	  	:   无
 */
static void set_gpio_input(void)
{
    gpio_direction_input(ds18b20.gpio);
}
/*
 * @description	: 	获取GPIO的值
 * @param 		:	无 
 * @return 	  	:   GPIO的电平
 */
static int get_gpio_value(void)
{
    return gpio_get_value(ds18b20.gpio); 
}

/*
 * @description	: 	初始化DS18B20
 * @param 		: 	无
 * @return 	  	:   0，初始化成功，1，失败
 */
static int ds18b20_init(void)
{
    int ret = -1;
	set_gpio_output(HIGH);	/* 把DQ拉高 */
	udelay(1);					/* 拉高1us */
	set_gpio_output(LOW);	/* 把DQ拉低 */
    udelay(500);				/* 拉低500us */
	set_gpio_output(HIGH);	/* 把DQ拉高 */
	udelay(60);					/* 拉高60us */
	set_gpio_input();		/* 设置为输入模式 */
    ret = get_gpio_value();		/* 获取到低电平做响应 */
	udelay(240);				/* 延时240us */
	
	return ret;		
}
/*
 * @description	: 	写一位数据
 * @param 	bit	: 	要写入的位数
 * @return 	  	:   无
 */
static void ds18b20_write_bit(int bit)
{
    local_irq_disable();
    if(1 == bit) {
        set_gpio_output(LOW);  /* 把DQ拉低 */
        udelay(2);				  /* 拉低2us */
        set_gpio_output(HIGH); /* 把DQ拉高 */
        udelay(60); 			  /* 拉高60us */
    } else {
        set_gpio_output(LOW);  /* 把DQ拉低 */
        udelay(60);				  /* 拉低60us */
        set_gpio_output(HIGH); /* 把DQ拉高 */
        udelay(2);				  /* 拉高2us */
    }
    local_irq_enable();
}
/*
 * @description	: 	读一位数据
 * @param 		: 	无
 * @return 	  	:   返回读取一位的数据
 */
static int ds18b20_read_bit(void)
{
    // int ret;
    unsigned char bit = 0;
    local_irq_disable();
    set_gpio_output(LOW);  /* 把DQ拉低 */
    udelay(2);
    set_gpio_output(HIGH);	/* 把DQ拉高 */
    udelay(1);					/* 拉高1us */

    set_gpio_input();		/* 设置为输入模式 */

    if(get_gpio_value())		/* 获取DQ的电平，高为1，低为0 */
        bit = 1;

    udelay(50);
    local_irq_enable();

    return bit;
}

/*
 * @description	: 	写一个字节到DS18B20
 * @param byte  : 	要写入的字节
 * @return 	  	:   无
 */
static void ds18b20_write_byte(unsigned char byte)
{
    int i;
    for(i = 0; i < 8; i++) {
        if(byte & 0x01)
            ds18b20_write_bit(1); /* write 1 */
        else
            ds18b20_write_bit(0); /* write 0 */
        byte >>= 1;	/* 右移一位获取高一位的数据 */
    }
}
/*
 * @description	: 	读取一个字节的数据
 * @param 		: 	无
 * @return 	  	:   读取到的数据
 */
static char ds18b20_read_byte(void)
{
    int i;
    unsigned char byte = 0;
    for(i = 0; i < 8; i++) {	/* DS18B20先输出低位数据 ,高位数据后输出 */
        if(ds18b20_read_bit())
            byte |= (1 << i);
        else
            byte &= ~(1 << i);
    }
    return byte;
}

static int ds18b20_open(struct inode *inode, struct file *filp)
{
	return 0;
}
static ssize_t ds18b20_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int ret;
    ret = copy_to_user(buf, &ds18b20.data[0], 2);
    if(ret)
        return -ENOMEM;
    return ret;
}

static struct file_operations ds18b20_fops = {
    .owner   = THIS_MODULE,
    .open   = ds18b20_open,
    .read   = ds18b20_read,
};

/*
 * @description     : 使用内核的工作队列，获取温度的原始数据
 * @param - work 	: work的结构体
 * @return          : 无
 */
static void ds18b20_work_callback(struct work_struct *work)
{
    int ret = -1;
    // 第一阶段
    ret = ds18b20_init();
    if(ret != 0)
        goto out1;
    ds18b20_write_byte(0XCC);	/* 跳过ROM */
	ds18b20_write_byte(0X44);	/* 开启温度采集 */
    // udelay(1);

    // 第二阶段
    ret = ds18b20_init();		/* 初始化ds18b20 */
     if(ret != 0)
         goto out1;
	
	ds18b20_write_byte(0XCC);	/* 跳过ROM */
	ds18b20_write_byte(0XBE);	/* 开启读取温度 */
	ds18b20.data[0] = ds18b20_read_byte();	/* 获取低位数据 */
	ds18b20.data[1] = ds18b20_read_byte();	/* 获取高位数据 */

out1:
    return;
}

static void ds18b20_timer_callback(struct timer_list *arg)
{
    schedule_work(&ds18b20.work);
    mod_timer(&ds18b20.timer, jiffies + (1000 * HZ/1000));
}

static int ds18b20_gpio_init(struct platform_device *pdev)
{
    struct device *dev = &pdev -> dev;
    int ret;

    ds18b20.gpio = of_get_named_gpio(dev->of_node, "ds18b20-gpio", 0);
    if(!gpio_is_valid(ds18b20.gpio))
    {
        dev_err(dev, "failed to get gpio");
        return -EINVAL;
    }
    printk("%d\n\r",  ds18b20.gpio);
    ret = devm_gpio_request(dev, ds18b20.gpio, "ds18b20");
    if(ret)
    {
        dev_err(dev, "failed to request gpio");
        return ret;
    }

    return 0;
}


static int ds18b20_probe(struct platform_device *pdev)
{
    int ret;
    struct miscdevice *mdev;

    dev_info(&pdev->dev, "matched success\n\r");
    // dev_info(&pdev->dev, "matched 1");

    ret = ds18b20_gpio_init(pdev);
    if(ret) return ret;
    dev_info(&pdev->dev, "matched 2");

    mdev = &ds18b20.ds18b20_miscdev;
    mdev->name = "ds18b20";
    mdev->minor = MISC_DYNAMIC_MINOR;
    mdev->fops = &ds18b20_fops;
    // dev_info(&pdev->dev, "matched 3");

    timer_setup(&ds18b20.timer, ds18b20_timer_callback, 0);
    ds18b20.timer.expires = jiffies + msecs_to_jiffies(1000);
    add_timer(&ds18b20.timer);
    // dev_info(&pdev->dev, "matched 4");

    INIT_WORK(&ds18b20.work, ds18b20_work_callback);
    // dev_info(&pdev->dev, "matched 5");
    ret = misc_register(mdev);

    return ret;
}

static int ds18b20_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "DS18B20 driver has been removed!\n");

    misc_deregister(&ds18b20.ds18b20_miscdev);
    del_timer(&ds18b20.timer);
    cancel_work_sync(&ds18b20.work);

    return 0;
}

static const struct of_device_id ds18b20_of_match[] = {
    {.compatible = "zave,ds18b20"},
    {}
};

static struct platform_driver ds18b20_driver = {
    .driver = {
		.name = "ds18b20",
        .of_match_table	= ds18b20_of_match,
	},
	.probe = ds18b20_probe,
    .remove = ds18b20_remove,
};

module_platform_driver(ds18b20_driver);

MODULE_DESCRIPTION("DS18B20 Driver, buy form zave");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("WuYue");
MODULE_INFO(intree, "Y");
