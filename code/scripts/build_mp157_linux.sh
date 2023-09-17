#!/bin/sh

cd ../linux/linux-stm32mp-5.4.31-r0/linux-5.4.31

# ./arch/arm/configs/stm32mp1_atk_defconfig
# make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabihf- distclean

# make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabihf- stm32mp1_atk_defconfig
make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabihf- uImage dtbs LOADADDR=0XC2000040 -j16

cp arch/arm/boot/uImage ../../../../kernel_dtb/uImage
cp arch/arm/boot/dts/stm32mp157d-atk.dtb ../../../../kernel_dtb/stm32mp157d-atk.dtb
sync

echo -------------------------------------------------
echo tftp get this files
echo -------------------------------------------------

ls -al ../../../../kernel_dtb/stm32mp157d-atk.dtb
ls -al ../../../../kernel_dtb/uImage





# make uImage LOADADDR=0XC2000040
# make dtbs

# 'tftp c2000000 uImage;tftp c4000000 stm32mp157d-atk.dtb;bootm c2000000 - c4000000'