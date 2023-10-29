#include <iostream>
#include <vector>
#include "common.h"

// 抽象基类
class Device {
protected:
    std::string device_name;

public:
    virtual int readData(int *) = 0;
    virtual int writeData(int *) = 0;
    void resetDevice();
    void printDeviceInfo();
};

// 具体类 - SPI Flash
class SPIFlash : public Device {
public:
    SPIFlash(std::string device_name);
    int readData(int *) override;
    int writeData(int *) override;
    void resetDevice();
    void printDeviceInfo();
};

// 具体类 - EEPROM
class EEPROM : public Device {
public:
    EEPROM(std::string device_name);
    int readData(int *) override;
    int writeData(int *) override;
    void resetDevice();
    void printDeviceInfo();
};

// 具体类 - 温度传感器
class TemperatureSensor : public Device {
public:
    TemperatureSensor(std::string device_name);
    int readData(int *) override;
    int writeData(int *) override;
    void resetDevice();
    void printDeviceInfo();
};
