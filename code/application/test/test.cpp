#include <iostream>
#include <fstream>
#include <vector>
#include "common.h"
#include "test.h"


// 抽象基类
void Device::resetDevice() {
    std::cout << "Resetting the device." << std::endl;
    // 实现重置设备的逻辑
}
void Device::printDeviceInfo() {
    std::cout << "Device information:" << std::endl;
    // 实现打印设备信息的逻辑
}

//SPI Flash
SPIFlash::SPIFlash(std::string device_name) {
    device_name = device_name;
}
void SPIFlash::resetDevice() {
    std::cout << "Resetting SPIFlash : w25q64." << std::endl;
}
void SPIFlash::printDeviceInfo() {
    std::cout << "Device information:" << std::endl;
    std::cout << "device_name:" << device_name << std::endl;
}
int SPIFlash::readData(int *temp)
{
    system("cat /home/w25q64/test.txt");
    std::cout << std::endl;
}
int SPIFlash::writeData(int *temp) {
    system("touch /home/w25q64/test.txt");
    std::ofstream outputFile("/home/w25q64/test.txt");
    if (outputFile.is_open()) {
        outputFile << "hello test demo";
        outputFile.close();
        std::cout << "写入成功！" << std::endl;
    } else {
        std::cout << "无法打开文件。" << std::endl;
    }

    return 0;
}
//EEPROM
EEPROM::EEPROM(std::string device_name) {
    device_name = device_name;
}
void EEPROM::resetDevice() {
    std::cout << "Resetting EEPROM : at24c08." << std::endl;
}
void EEPROM::printDeviceInfo() {
    std::cout << "Device information:" << std::endl;
    std::cout << "device_name:" << device_name << std::endl;
}
int EEPROM::readData(int *reg)
{
    int rst = 0;
    unsigned char buff = -1;
    rst = BspReadEeprom(0x30, 1, &buff);
    // if rst
    std::cout << "Reading data from EEPROM (Reg: " << *reg << " is " << buff << ")." << std::endl;
    return buff;
}
int EEPROM::writeData(int *reg) {
    int rst = 0;
    unsigned char buff = 'E';
    std::cout << "writeData to EEPROM (Reg: " << *reg << ")." << std::endl;
    BspWriteEeprom(0x30, 1, &buff);
    rst = readData(reg);
    std::cout << "writeData rst " << buff << std::endl;
    return 0XAA == rst;
}

/**** 温度传感器****/
TemperatureSensor::TemperatureSensor(std::string device_name) {
    device_name = device_name;
}
void TemperatureSensor::resetDevice() {
    std::cout << "Resetting TemperatureSensor : DS18B20." << std::endl;
}
void TemperatureSensor::printDeviceInfo() {
    std::cout << "Device information:" << std::endl;
    std::cout << "device_name:" << device_name << std::endl;
}
int TemperatureSensor::readData(int *temp)
{
    *temp = BspGetTemp();
    std::cout << "temperature Sensor read data:" << *temp << std::endl;
    return 0;
}
int TemperatureSensor::writeData(int *)
{
    std::cout << "temperature Sensor dont need write data:" << std::endl;
    return 0;
}
