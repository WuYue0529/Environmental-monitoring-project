#!/bin/sh

cd ../uboot/u-boot-stm32mp-2020.01-r0/u-boot-stm32mp-2020.01/
# for p in `ls -1 ../*.patch`;do patch -p1 < $p;done

make distclean
make stm32mp15_atk_trusted_defconfig
make DEVICE_TREE=stm32mp157d-atk all -j12

# setenv ipaddr 192.168.31.249
# setenv ethaddr 00:04:9f:04:d2:35
# setenv gatewayip 192.168.31.1
# setenv netmask 255.255.255.0
# setenv serverip 192.168.31.177
# saveenv

# setenv bootargs 'console=ttySTM0,115200 root=/dev/nfs nfsroot=192.168.31.177:/home/charles/linux/rootfs/mp157d_atk_buildroot,proto=tcp rw ip=192.168.31.249:192.168.31.177:192.168.31.1:255.255.255.0::eth0:off'
