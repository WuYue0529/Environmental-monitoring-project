#!/bin/sh
make clean
cmake ..
make
echo "complier end ......"
echo "---------------------------------"

cp energetic ../../../rootfs/mp157d_atk_buildroot/ -f
sync
