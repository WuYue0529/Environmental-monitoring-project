#!/bin/sh

make 
cp at24c08.ko ../../../rootfs/mp157d_atk_buildroot/lib/modules/5.4.31/ -f
sync
make clean