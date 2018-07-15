#include "i2csupervisor.h"
#include <iostream>
#include <wiringPiI2C.h>

i2cSupervisor::i2cSupervisor()
{

}

int i2cSupervisor::I2CRead(int fd)
{
    return wiringPiI2CRead(fd);
}

int i2cSupervisor::I2CReadReg8(int fd, int reg)
{
    return wiringPiI2CReadReg8(fd,reg);
}

int i2cSupervisor::I2CReadReg16(int fd, int reg)
{
    return wiringPiI2CReadReg16(fd,reg);
}

int i2cSupervisor::I2CWrite(int fd, int data)
{
    return wiringPiI2CWrite(fd,data);
}

int i2cSupervisor::I2CWriteReg8(int fd, int reg, int data)
{
    return wiringPiI2CWriteReg8(fd, reg, data);
}

int i2cSupervisor::I2CWriteReg16(int fd, int reg, int data)
{
    return wiringPiI2CWriteReg16(fd, reg, data);
}

int i2cSupervisor::I2CSetupInterface(const char *device, int devId)
{
    return wiringPiI2CSetupInterface(device,devId);
}

int i2cSupervisor::I2CSetup(const int devId)
{
    std::cout<<"Opening i2c device "<<devId<<std::endl;
    return wiringPiI2CSetup(devId);
}

void i2cSupervisor::IICwriteBit(int dev, char reg, char bitNum, char data){
    char b;
    b = I2CReadReg8(dev,reg);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    I2CWriteReg8(dev, reg, b);
}

void i2cSupervisor::IICwriteBits(int dev, char reg, char bitStart, char length, char data)
{

    char b,mask;
    b = I2CReadReg8(dev,reg);
    mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
    data <<= (8 - length);
    data >>= (7 - bitStart);
    b &= mask;
    b |= data;
    I2CWriteReg8(dev, reg, b);
}

void i2cSupervisor::IICreadBytes(int fd, int start, int size, char *data)
{
    for(int i=0;i<size;++i)
    {
        data[i]=(char) I2CReadReg8(fd,start+i);
    }
}
