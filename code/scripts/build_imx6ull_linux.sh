#!/bin/sh

cd ../linux/linux-imx-rel_imx_4.1.15_2.1.0_ga

# make distclean
make imx_alientek_emmc_defconfig
make  -j16

cp arch/arm/boot/zImage ../../../kernel_dtb/ -f
cp arch/arm/boot/dts/imx6ull-alientek-emmc.dtb ../../../kernel_dtb/ -f
sync

