cmake_minimum_required(VERSION 3.0)
project(YourProjectName)

set(CMAKE_C_COMPILER "/usr/local/arm/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "/usr/local/arm/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-g++")

# 添加源文件
# 收集目录中的文件名并赋值给变量
aux_source_directory(src DIR_SRCS)
add_executable(energetic main.cpp ${DIR_SRCS})

# 添加头文件路径
include_directories(inc)
# include_directories(test)
include_directories(lib/tslib/include)
include_directories(lib/jpeglib/include)
include_directories(lib/pnglib/include)
include_directories(lib/freetype/include/freetype2)
include_directories(lib/freetype/include/freetype2/freetype)
include_directories(lib/mqttlib/include)

find_library(TSLIB_LIB NAMES ts HINTS /home/zk/linux/code/application/lib/tslib/lib)
find_library(JPEGLIB_LIB NAMES jpeg HINTS /home/zk/linux/code/application/lib/jpeglib/lib)
# find_library(JPEG_LIBRARY jpeg HINTS /home/zk/linux/code/application/lib/jpeglib/lib)  # 区别？
find_library(ZLIB_LIB NAMES z HINTS /home/zk/linux/code/application/lib/zlib/lib)
find_library(PNGLIB_LIB NAMES png HINTS /home/zk/linux/code/application/lib/pnglib/lib)
find_library(FREETYPE_LIB NAMES freetype HINTS /home/zk/linux/code/application/lib/freetype/lib)
find_library(MQTT_LIB NAMES paho-mqtt3c HINTS /home/zk/linux/code/application/lib/mqttlib/lib)

find_package(Threads REQUIRED)

target_link_libraries(energetic ${TSLIB_LIB} ${JPEG_LIBRARY} ${ZLIB_LIB} ${PNGLIB_LIB} ${FREETYPE_LIB} ${MQTT_LIB} Threads::Threads)

# 指定编译输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
