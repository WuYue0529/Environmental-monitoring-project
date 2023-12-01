set(CMAKE_SYSTEM_NAME Linux) #设置目标系统名字
set(CMAKE_SYSTEM_PROCESSOR arm) #设置目标处理器架构

set(TOOLCHAIN_DIR /opt/st/stm32mp1/3.1-snapshot/sysroots)
set(CMAKE_SYSROOT ${TOOLCHAIN_DIR}/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi)

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc)

set(CMAKE_C_FLAGS "-mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/home/zk/linux/code/application/lib/mqttlib -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-linux-setup.cmake -DPAHO_WITH_SSL=TRUE -DPAHO_BUILD_SAMPLES=TRUE ..
# source /opt/st/stm32mp1/3.1-snapshot/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
