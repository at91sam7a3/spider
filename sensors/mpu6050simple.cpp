#include "mpu6050simple.h"
#include "i2csupervisor.h"
#include <math.h>
#include <iostream>
#define PI 3.141592654
mpu6050simple::mpu6050simple(int addr)
{
    address_= addr;
    i2c = i2cSupervisor::I2CSetup(addr);
    i2cSupervisor::I2CWriteReg8(i2c,0x6b,0);
}

void mpu6050simple::getRotation(float &x_angle, float &y_angle)
{
    // int gyro_xout = i2cSupervisor::I2CReadReg16 (i2c, 0x43);
    //  int gyro_yout = i2cSupervisor::I2CReadReg16 (i2c, 0x45);
    // int gyro_zout = i2cSupervisor::I2CReadReg16 (i2c, 0x47);


    float accel_xout = read_word_2c(0x3b);
    float accel_yout = read_word_2c(0x3d);
    float accel_zout = read_word_2c(0x3f);
    //std::cout<<"accel x = "<<accel_xout<<std::endl;
    float accel_xout_scaled = (float)accel_xout / 16384.0;
    float accel_yout_scaled = (float)accel_yout / 16384.0;
    float accel_zout_scaled = (float)accel_zout / 16384.0;

    x_angle =  get_x_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled);
    y_angle =  get_y_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled);
}

float mpu6050simple::dist(float x, float y){
    return sqrt(x*x + y*y);
}

float mpu6050simple::get_y_rotation(float x, float y, float z){

    float radians = atan2(x, dist(y,z));
    return radians*180.0/PI;
}

float mpu6050simple::get_x_rotation(float x, float y, float z){
    float radians = atan2(y, dist(x,z));
    return radians*180.0/PI;
}

int mpu6050simple::read_word(char adr){
    int high = i2cSupervisor::I2CReadReg8(i2c, adr);
    int low= i2cSupervisor::I2CReadReg8(i2c, adr+1);
    int val = (high << 8) + low;
    return val;
}

int mpu6050simple::read_word_2c(char adr){
    int val = read_word(adr);
    if (val >= 0x8000)
        return -((65535 - val) + 1);
    else
        return val;
}
