#!/bin/sh

make 
cp w25q64.ko ../../../rootfs/mp157d_atk_buildroot/lib/modules/5.4.31/ -f
sync
make clean