#!/bin/sh

cd ../tfa/mp157_alientek/tf-a-stm32mp-2.2.r1-r0/
cd tf-a-stm32mp-2.2.r1/

make -f ../Makefile.sdk clean
make -f ../Makefile.sdk all

# make -f ../Makefile.sdk clean
# make -f ../Makefile.sdk TFA_DEVICETREE=stm32mp157d-atk TF_A_CONFIG=serialboot ELF_DEBUG_ENABLE='1' all

