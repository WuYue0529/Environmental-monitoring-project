cmake_minimum_required(VERSION 3.0)
project(YourProjectName)

set(CMAKE_C_COMPILER "/usr/local/arm/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "/usr/local/arm/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-g++")

# 添加源文件
# 添加源文件
aux_source_directory(src DIR_SRCS)
add_executable(energetic main.cpp ${DIR_SRCS})

# add_executable(energetic  main.cpp src/ds18b20.cpp)

# 添加头文件路径
include_directories(inc)
find_package(Threads REQUIRED)
target_link_libraries(energetic Threads::Threads)
# 指定编译输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# 其他配置...

