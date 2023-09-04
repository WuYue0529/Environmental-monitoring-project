#!/bin/sh

cd ../busybox/imx6ull_alientek_alpha/busybox-1.29.0

# make distclean
# make imx_alientek_emmc_defconfig
make
make install CONFIG_PREFIX=/home/charles/linux/rootfs/imx_6ull_alientek_alpha

# cp arch/arm/boot/zImage ../../../kernel_dtb/ -f
# sync
