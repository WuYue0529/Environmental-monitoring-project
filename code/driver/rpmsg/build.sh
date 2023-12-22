#!/bin/sh

make 
cp led.ko ../../../rootfs/mp157d_atk_buildroot/lib/modules/5.4.31/ -f
sync
make clean