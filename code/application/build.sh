#!/bin/sh
arm-none-linux-gnueabihf-g++ env.cpp -o app
cp app ../../rootfs/mp157d_atk_buildroot/lib/modules/5.4.31/ -f
sync