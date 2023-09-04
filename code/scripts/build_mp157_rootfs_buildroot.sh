#!/bin/sh

cd ../buildroot/buildroot-2020.02.6

# make distclean
# make stm32mp1_atk_defconfig
make
# rm -rf ../../../rootfs/mp157d_atk_buildroot/*
mkdir -p ../../../rootfs/mp157d_atk_buildroot/
tar -xvf ./output/images/rootfs.tar -C ../../../rootfs/mp157d_atk_buildroot/
sync
