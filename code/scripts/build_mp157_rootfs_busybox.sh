#!/bin/sh

cd ../busybox/mp157d_atk/busybox-1.32.0

# make distclean
# make imx_alientek_emmc_defconfig
make
make install CONFIG_PREFIX=/home/charles/linux/rootfs/mp157d_atk

# cp arch/arm/boot/zImage ../../../kernel_dtb/ -f
# sync
# 'console=ttySTM0,115200 root=/dev/nfs nfsroot=192.168.31.177:/home/charles/linux/rootfs/mp157d_atk,proto=tcp rw ip=192.168.31.249:192.168.31.177:192.168.31.1:255.255.255.0::eth0:off'