#!/bin/sh

cd ../uboot/uboot-imx-rel_imx_4.1.15_2.1.0_ga

# make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
make mx6ull_alientek_emmc_defconfig
make V=1 -j16

