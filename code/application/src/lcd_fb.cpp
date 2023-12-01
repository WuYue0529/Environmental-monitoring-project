#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <iostream>
#include <string.h>
#include <jpeglib.h>




void lcdscreeninfoshow()
{
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;
    int fd;

    /* 打开framebuffer设备 */
    if (0 > (fd = open("/dev/fb0", O_WRONLY))) {
        perror("open error");
        exit(-1);
    }

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);
    printf("分辨率: %d*%d\n"
        "像素深度bpp: %d\n"
        "一行的字节数: %d\n"
        "像素格式: R<%d %d> G<%d %d> B<%d %d>\n",
        fb_var.xres, fb_var.yres, fb_var.bits_per_pixel,
        fb_fix.line_length,
        fb_var.red.offset, fb_var.red.length,
        fb_var.green.offset, fb_var.green.length,
        fb_var.blue.offset, fb_var.blue.length);

    /* 关闭设备文件退出程序 */
    close(fd);
}

#define argb8888_to_rgb565(color)   ({ \
            unsigned int temp = (color); \
            ((temp & 0xF80000UL) >> 8) | \
            ((temp & 0xFC00UL) >> 5) | \
            ((temp & 0xF8UL) >> 3); \
            })

static int width;                   //LCD X分辨率
static int height;                      //LCD Y分辨率
static unsigned short *screen_base = NULL;      //映射后的显存基地址
/********************************************************************
 * 函数名称： lcd_draw_point
 * 功能描述： 打点
 * 输入参数： x, y, color
 * 返 回 值： 无
 ********************************************************************/
static void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color);//得到RGB565颜色值

    /* 对传入参数的校验 */
    if (x >= width)
        x = width - 1;
    if (y >= height)
        y = height - 1;

    /* 填充颜色 */
    screen_base[y * width + x] = rgb565_color;
}

/********************************************************************
 * 函数名称： lcd_draw_line
 * 功能描述： 画线（水平或垂直线）
 * 输入参数： x, y, dir, length, color
 * 返 回 值： 无
 ********************************************************************/
static void lcd_draw_line(unsigned int x, unsigned int y, int dir,
            unsigned int length, unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color);//得到RGB565颜色值
    unsigned int end;
    unsigned long temp;

    /* 对传入参数的校验 */
    if (x >= width)
        x = width - 1;
    if (y >= height)
        y = height - 1;

    /* 填充颜色 */
    temp = y * width + x;//定位到起点
    if (dir) {  //水平线
        end = x + length - 1;
        if (end >= width)
            end = width - 1;

        for ( ; x <= end; x++, temp++)
            screen_base[temp] = rgb565_color;
    }
    else {  //垂直线
        end = y + length - 1;
        if (end >= height)
            end = height - 1;

        for ( ; y <= end; y++, temp += width)
            screen_base[temp] = rgb565_color;
    }
}

/********************************************************************
 * 函数名称： lcd_draw_rectangle
 * 功能描述： 画矩形
 * 输入参数： start_x, end_x, start_y, end_y, color
 * 返 回 值： 无
 ********************************************************************/
static void lcd_draw_rectangle(unsigned int start_x, unsigned int end_x,
            unsigned int start_y, unsigned int end_y,
            unsigned int color)
{
    int x_len = end_x - start_x + 1;
    int y_len = end_y - start_y - 1;

    lcd_draw_line(start_x, start_y, 1, x_len, color);//上边
    lcd_draw_line(start_x, end_y, 1, x_len, color); //下边
    lcd_draw_line(start_x, start_y + 1, 0, y_len, color);//左边
    lcd_draw_line(end_x, start_y + 1, 0, y_len, color);//右边
}

/********************************************************************
 * 函数名称： lcd_fill
 * 功能描述： 将一个矩形区域填充为参数color所指定的颜色
 * 输入参数： start_x, end_x, start_y, end_y, color
 * 返 回 值： 无
 ********************************************************************/
static void lcd_fill(unsigned int start_x, unsigned int end_x,
            unsigned int start_y, unsigned int end_y,
            unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color);//得到RGB565颜色值
    unsigned long temp;
    unsigned int x;

    /* 对传入参数的校验 */
    if (end_x >= width)
        end_x = width - 1;
    if (end_y >= height)
        end_y = height - 1;

    /* 填充颜色 */
    temp = start_y * width; //定位到起点行首
    std::cout << "fill" << std::endl;
    for ( ; start_y <= end_y; start_y++, temp+=width)
    {
        // std::cout << "start_y" << std::endl;
        for (x = start_x; x <= end_x; x++)
        {
            // std::cout << "start_x" << std::endl;
            // std::cout << screen_base[0] << std::endl;
            screen_base[temp + x] = rgb565_color;
            // std::cout << screen_base[temp + x] << std::endl;
        }
    }
    std::cout << "fill-----------" << std::endl;

}

void lcdscreentest()
{
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;
    unsigned int screen_size;
    int fd;

    /* 打开framebuffer设备 */
    if (0 > (fd = open("/dev/fb0", O_RDWR))) {
        perror("open error");
        exit(EXIT_FAILURE);
    }
    std::cout << fd << std::endl;

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);

    screen_size = fb_fix.line_length * fb_var.yres;
    width = fb_fix.line_length / (fb_var.bits_per_pixel / 8);
    height = fb_var.yres;

    std::cout << screen_size << std::endl;
    std::cout << screen_size << std::endl;
    /* 将显示缓冲区映射到进程地址空间 */
    screen_base = reinterpret_cast<unsigned short*>(mmap(NULL, screen_size, PROT_WRITE, MAP_SHARED, fd, 0));
    if (MAP_FAILED == (void *)screen_base) {
        perror("mmap error");
        close(fd);
        exit(EXIT_FAILURE);
    }
    std::cout << screen_base << std::endl;
    std::cout << screen_base + 1 << std::endl;

    /* 画正方形方块 */
    int w = height * 0.25;//方块的宽度为1/4屏幕高度
    lcd_fill(0, fb_var.xres-1, 0, height-1, 0x0); //清屏（屏幕显示黑色）
    std::cout << 222 << std::endl;
    lcd_fill(0, w, 0, w, 0xFF0000); //红色方块
    lcd_fill(fb_var.xres-w, fb_var.xres-1, 0, w, 0xFF00);   //绿色方块
    lcd_fill(0, w, height-w, height-1, 0xFF);   //蓝色方块
    lcd_fill(fb_var.xres-w, fb_var.xres-1, height-w, height-1, 0xFFFF00);//黄色方块

    /* 画线: 十字交叉线 */
    lcd_draw_line(0, height * 0.5, 1, fb_var.xres, 0xFFFFFF);//白色线
    lcd_draw_line(fb_var.xres * 0.5, 0, 0, height, 0xFFFFFF);//白色线

    /* 画矩形 */
    unsigned int s_x, s_y, e_x, e_y;
    s_x = 0.25 * fb_var.xres;
    s_y = w;
    e_x = fb_var.xres - s_x;
    e_y = height - s_y;

    for ( ; (s_x <= e_x) && (s_y <= e_y);
            s_x+=5, s_y+=5, e_x-=5, e_y-=5)
        lcd_draw_rectangle(s_x, e_x, s_y, e_y, 0xFFFFFF);

    /* 退出 */
    munmap(screen_base, screen_size);  //取消映射
    close(fd);  //关闭文件
    exit(EXIT_SUCCESS); //退出进程
}



/**** BMP文件头数据结构 ****/
typedef struct {
    unsigned char type[2];      //文件类型
    unsigned int size;          //文件大小
    unsigned short reserved1;   //保留字段1
    unsigned short reserved2;       //保留字段2
    unsigned int offset;        //到位图数据的偏移量
} __attribute__ ((packed)) bmp_file_header;

/**** 位图信息头数据结构 ****/
typedef struct {
    unsigned int size;          //位图信息头大小
    int width;                  //图像宽度
    int height;                 //图像高度
    unsigned short planes;          //位面数
    unsigned short bpp;         //像素深度 
    unsigned int compression;   //压缩方式
    unsigned int image_size;    //图像大小
    int x_pels_per_meter;       //像素/米
    int y_pels_per_meter;       //像素/米 
    unsigned int clr_used;
    unsigned int clr_omportant;
} __attribute__ ((packed)) bmp_info_header;

/**** 静态全局变量 ****/
static struct fb_fix_screeninfo fb_fix;
static struct fb_var_screeninfo fb_var;

/********************************************************************
 * 函数名称： show_bmp_image
 * 功能描述： 在LCD上显示指定的BMP图片
 * 输入参数： 文件路径
 * 返 回 值： 成功返回0, 失败返回-1
 ********************************************************************/
static int show_bmp_image(const char *path)
{
    bmp_file_header file_h;
    bmp_info_header info_h;
    unsigned short *line_buf = NULL;    //行缓冲区
    unsigned long line_bytes;   //BMP图像一行的字节的大小
    unsigned int min_h, min_bytes;
    unsigned short *base = screen_base;
    unsigned int bytes_per_pixel = fb_var.bits_per_pixel / 8;
    unsigned int width = fb_fix.line_length / bytes_per_pixel;
    unsigned int line_length = bytes_per_pixel * fb_var.xres;
    int fd = -1;
    int j;

    /* 打开文件 */
    if (0 > (fd = open(path, O_RDONLY))) {
        perror("open error");
        return -1;
    }

    /* 读取BMP文件头 */
    if (sizeof(bmp_file_header) !=
        read(fd, &file_h, sizeof(bmp_file_header))) {
        perror("read error");
        close(fd);
        return -1;
    }

    if (0 != memcmp(file_h.type, "BM", 2)) {
        fprintf(stderr, "it's not a BMP file\n");
        close(fd);
        return -1;
    }

    /* 读取位图信息头 */
    if (sizeof(bmp_info_header) !=
        read(fd, &info_h, sizeof(bmp_info_header))) {
        perror("read error");
        close(fd);
        return -1;
    }

    /* 打印信息 */
    printf("文件大小: %d\n"
         "位图数据的偏移量: %d\n"
         "位图信息头大小: %d\n"
         "图像分辨率: %d*%d\n"
         "像素深度: %d\n", file_h.size, file_h.offset,
         info_h.size, info_h.width, info_h.height,
         info_h.bpp);

    /* 将文件读写位置移动到图像数据开始处 */
    if (-1 == lseek(fd, file_h.offset, SEEK_SET)) {
        perror("lseek error");
        close(fd);
        return -1;
    }

    /* 申请一个buf、暂存bmp图像的一行数据 */
    line_bytes = info_h.width * info_h.bpp / 8;
    line_buf = (unsigned short *)malloc(line_bytes);
    if (NULL == line_buf) {
        fprintf(stderr, "malloc error\n");
        close(fd);
        return -1;
    }

    if (line_length > line_bytes)
        min_bytes = line_bytes;
    else
        min_bytes = line_length;

    /**** 读取图像数据显示到LCD ****/
    /*******************************************
     * 为了软件处理上方便，这个示例代码便不去做兼容性设计了
     * 如果你想做兼容, 可能需要判断传入的BMP图像是565还是888
     * 如何判断呢？文档里边说的很清楚了
     * 我们默认传入的bmp图像是RGB565格式
     *******************************************/
    if (0 < info_h.height) {//倒向位图
        if (info_h.height > fb_var.yres) {
            min_h = fb_var.yres;
            lseek(fd, (info_h.height - fb_var.yres) * line_bytes, SEEK_CUR);
            base += width * (fb_var.yres - 1);    //定位到屏幕左下角位置
        }
        else {
            min_h = info_h.height;
            base += width * (info_h.height - 1); //定位到....不知怎么描述 懂的人自然懂!
        }

        for (j = min_h; j > 0; base -= width, j--) {
            read(fd, line_buf, line_bytes); //读取出图像数据
            memcpy(base, line_buf, min_bytes);//刷入LCD显存
        }
    }
    else {  //正向位图
        int temp = 0 - info_h.height;   //负数转成正数
        if (temp > fb_var.yres)
            min_h = fb_var.yres;
        else
            min_h = temp;

        for (j = 0; j < min_h; j++, base += width) {
            read(fd, line_buf, line_bytes);
            memcpy(base, line_buf, min_bytes);
        }
    }

    /* 关闭文件、函数返回 */
    close(fd);
    free(line_buf);
    return 0;
}

int bmpshow()
{
    unsigned int screen_size;
    int fd;

    /* 打开framebuffer设备 */
    if (0 > (fd = open("/dev/fb0", O_RDWR))) {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);
    screen_size = fb_fix.line_length * fb_var.yres;

    /* 将显示缓冲区映射到进程地址空间 */
    screen_base = reinterpret_cast<unsigned short*>(mmap(NULL, screen_size, PROT_WRITE, MAP_SHARED, fd, 0));
    if (MAP_FAILED == (void *)screen_base) {
        perror("mmap error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* 显示BMP图片 */
    memset(screen_base, 0xFF, screen_size);
    show_bmp_image("/image.bmp");

    /* 退出 */
    munmap(screen_base, screen_size);  //取消映射
    close(fd);  //关闭文件
    exit(EXIT_SUCCESS);    //退出进程
}

typedef struct bgr888_color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} __attribute__ ((packed)) bgr888_t;

static int show_jpeg_image(const char *path)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *jpeg_file = NULL;
    bgr888_t *jpeg_line_buf = NULL;     //行缓冲区:用于存储从jpeg文件中解压出来的一行图像数据
    unsigned short *fb_line_buf = NULL; //行缓冲区:用于存储写入到LCD显存的一行数据
    unsigned int min_h, min_w;
    unsigned int valid_bytes;
    unsigned int bytes_per_pixel = fb_var.bits_per_pixel / 8;
    unsigned int width = fb_fix.line_length / bytes_per_pixel;
    unsigned short *base = screen_base;
    int i;

    //绑定默认错误处理函数
    cinfo.err = jpeg_std_error(&jerr);

    //打开.jpeg/.jpg图像文件
    jpeg_file = fopen(path, "r");   //只读方式打开
    if (NULL == jpeg_file) {
        perror("fopen error");
        return -1;
    }

    //创建JPEG解码对象
    jpeg_create_decompress(&cinfo);

    //指定图像文件
    jpeg_stdio_src(&cinfo, jpeg_file);

    //读取图像信息
    jpeg_read_header(&cinfo, TRUE);
    printf("jpeg图像大小: %d*%d\n", cinfo.image_width, cinfo.image_height);

    //设置解码参数
    cinfo.out_color_space = JCS_RGB;//默认就是JCS_RGB
    //cinfo.scale_num = 1;
    //cinfo.scale_denom = 2;

    //开始解码图像
    jpeg_start_decompress(&cinfo);

    //为缓冲区分配内存空间
    jpeg_line_buf = (bgr888_t *)malloc(cinfo.output_components * cinfo.output_width);
    fb_line_buf = (unsigned short *)malloc(fb_fix.line_length);

    //判断图像和LCD屏那个的分辨率更低
    if (cinfo.output_width > fb_var.xres)
        min_w = fb_var.xres;
    else
        min_w = cinfo.output_width;

    if (cinfo.output_height > fb_var.yres)
        min_h = fb_var.yres;
    else
        min_h = cinfo.output_height;

    //读取数据
    valid_bytes = min_w * bytes_per_pixel;//一行的有效字节数 表示真正写入到LCD显存的一行数据的大小
    while (cinfo.output_scanline < min_h) {

        jpeg_read_scanlines(&cinfo, (unsigned char **)&jpeg_line_buf, 1);//每次读取一行数据

        //将读取到的BGR888数据转为RGB565
        for (i = 0; i < min_w; i++)
            fb_line_buf[i] = ((jpeg_line_buf[i].red & 0xF8) << 8) |
                    ((jpeg_line_buf[i].green & 0xFC) << 3) |
                    ((jpeg_line_buf[i].blue & 0xF8) >> 3);

        memcpy(base, fb_line_buf, valid_bytes);
        base += width;//+width  定位到LCD下一行显存地址的起点
    }

    //解码完成
    jpeg_finish_decompress(&cinfo); //完成解码
    jpeg_destroy_decompress(&cinfo);//销毁JPEG解码对象、释放资源

    //关闭文件、释放内存
    fclose(jpeg_file);
    free(fb_line_buf);
    free(jpeg_line_buf);
    return 0;
}


int jpegshow()
{
    unsigned int screen_size;
    int fd;

    /* 打开framebuffer设备 */
    if (0 > (fd = open("/dev/fb0", O_RDWR))) {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);
    screen_size = fb_fix.line_length * fb_var.yres;

    /* 将显示缓冲区映射到进程地址空间 */
    screen_base = (unsigned short *)mmap(NULL, screen_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == (void *)screen_base) {
        perror("mmap error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* 显示BMP图片 */
    memset(screen_base, 0xFF, screen_size);
    show_jpeg_image("/a.jpeg");

    /* 退出 */
    munmap(screen_base, screen_size);  //取消映射
    close(fd);  //关闭文件
    exit(EXIT_SUCCESS);    //退出进程
}
