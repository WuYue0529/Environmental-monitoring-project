#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <png.h>
#include <png_mte.h>
#include <string>



static struct fb_fix_screeninfo fb_fix;
static struct fb_var_screeninfo fb_var;
static unsigned short *screen_base = NULL;        //映射后的显存基地址

static int show_png_image(const char *path)
{
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    FILE *png_file = NULL;
    unsigned short *fb_line_buf = NULL; //行缓冲区:用于存储写入到LCD显存的一行数据
    unsigned int min_h, min_w;
    unsigned int valid_bytes;
    unsigned int image_h, image_w;
    png_bytepp row_pointers = NULL;
    unsigned int bytes_per_pixel = fb_var.bits_per_pixel / 8;
    unsigned int width = fb_fix.line_length / bytes_per_pixel;
    unsigned short *base = screen_base;
    int i, j, k;

    /* 打开png文件 */
    png_file = fopen(path, "r");    //以只读方式打开
    if (NULL == png_file) {
        perror("fopen error");
        return -1;
    }

    /* 分配和初始化png_ptr、info_ptr */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(png_file);
        return -1;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(png_file);
        return -1;
    }

    /* 设置错误返回点 */
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }

    /* 指定数据源 */
    png_init_io(png_ptr, png_file);

    /* 读取png文件 */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_ALPHA, NULL);
    image_h = png_get_image_height(png_ptr, info_ptr);
    image_w = png_get_image_width(png_ptr, info_ptr);
    printf("分辨率: %d*%d\n", image_w, image_h);

    /* 判断是不是RGB888 */
    if ((8 != png_get_bit_depth(png_ptr, info_ptr)) &&
        (PNG_COLOR_TYPE_RGB != png_get_color_type(png_ptr, info_ptr))) {
        printf("Error: Not 8bit depth or not RGB color");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }

    /* 判断图像和LCD屏那个的分辨率更低 */
    if (image_w > fb_var.xres)
        min_w = fb_var.xres;
    else
        min_w = image_w;

    if (image_h > fb_var.yres)
        min_h = fb_var.yres;
    else
        min_h = image_h;

    valid_bytes = min_w * bytes_per_pixel;

    /* 读取解码后的数据 */
    fb_line_buf = (unsigned short *)malloc(valid_bytes);
    row_pointers = png_get_rows(png_ptr, info_ptr);//获取数据

    unsigned int temp = min_w * 3;  //RGB888 一个像素3个bit位
    for(i = 0; i < min_h; i++) {

        // RGB888转为RGB565
        for(j = k = 0; j < temp; j += 3, k++)
            fb_line_buf[k] = ((row_pointers[i][j] & 0xF8) << 8) |
                ((row_pointers[i][j+1] & 0xFC) << 3) |
                ((row_pointers[i][j+2] & 0xF8) >> 3);

        memcpy(base, fb_line_buf, valid_bytes);//将一行数据刷入显存
        base += width;   //定位到显存下一行
    }

    /* 结束、销毁/释放内存 */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    free(fb_line_buf);
    fclose(png_file);
    return 0;
}

int pngshow(std::string path="a.png")
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
    memset(screen_base, 0xFF, screen_size);//屏幕刷白
    show_png_image(path.c_str());

    /* 退出 */
    munmap(screen_base, screen_size);  //取消映射
    close(fd);  //关闭文件
    exit(EXIT_SUCCESS);    //退出进程
}
