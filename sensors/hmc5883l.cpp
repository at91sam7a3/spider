#include <math.h>
#include "i2csupervisor.h"
#include "hmc5883l.h"

#define HMC_ConfigurationRegisterA 0x00
#define HMC_ConfigurationRegisterB 0x01
#define HMC_ModeRegister 0x02
#define HMC_AxisXDataRegisterMSB 0x03
#define HMC_AxisXDataRegisterLSB 0x04
#define HMC_AxisZDataRegisterMSB 0x05
#define HMC_AxisZDataRegisterLSB 0x06
#define HMC_AxisYDataRegisterMSB 0x07
#define HMC_AxisYDataRegisterLSB 0x08
#define HMC_StatusRegister 0x09
#define HMC_IdentificationRegisterA 0x10
#define HMC_IdentificationRegisterB 0x11
#define HMC_IdentificationRegisterC 0x12


#define HMC_MeasurementContinuous 0x00
#define HMC_MeasurementSingleShot 0x01
#define HMC_MeasurementIdle 0x03

#define PI 3.1415926

HMC5883l::HMC5883l( char addr, float gauss)
{
    address_= addr;
    i2c = i2cSupervisor::I2CSetup(addr);
    setScale(gauss);
}

void HMC5883l::setScale(float gauss)
{
    char scale_reg=0;
    float scale=0;
    if (gauss == 0.88){
        scale_reg = 0x00;
        scale = 0.73;
    }
    if (gauss == 1.3)
    {
        scale_reg = 0x01;
        scale = 0.92;
    }
    if (gauss == 1.9)
    {
        scale_reg = 0x02;
        scale = 1.22;
    }
    if (gauss == 2.5)
    {
        scale_reg = 0x03;
        scale = 1.52;
    }
    if (gauss == 4.0)
    {
        scale_reg = 0x04;
        scale = 2.27;
    }
    if (gauss == 4.7)
    {
        scale_reg = 0x05;
        scale = 2.56;
    }
    if (gauss == 5.6)
    {
        scale_reg = 0x06;
        scale = 3.03;
    }
    if (gauss == 8.1)
    {
        scale_reg = 0x07;
        scale = 4.35;
    }

    scale_reg = scale_reg << 5;
    setOption(HMC_ConfigurationRegisterB, scale_reg);
}

void HMC5883l::setOption(char reg, char data)
{
    i2cSupervisor::I2CWriteReg8(i2c,reg,data);
}

void HMC5883l::setContinuousMode()
{
    setOption(HMC_ModeRegister, HMC_MeasurementContinuous);
}

void HMC5883l::setDeclination(int degree, int min)
{
    declinationDeg_ = degree;
    declinationMin_ = min;
    declination_ = (degree+min/60) * (PI/180);
}

vec3f HMC5883l::getAxes()
{
    vec3f result;
    vec3i tmp;


    tmp.x = i2cSupervisor::I2CReadReg16(i2c, HMC_AxisXDataRegisterMSB);
    tmp.y = i2cSupervisor::I2CReadReg16(i2c, HMC_AxisYDataRegisterMSB);
    tmp.z = i2cSupervisor::I2CReadReg16(i2c, HMC_AxisZDataRegisterMSB);

    if (tmp.x == -4096)
        result.x = 0;
    else
        result.x = float(tmp.x) * scale;

    if (tmp.y == -4096)
        result.y = 0;
    else
        result.y = float(tmp.y) * scale;

    if (tmp.z == -4096)
        result.z = 0;
    else
        result.z = float(tmp.z) * scale;

    return result;
}

vec2i HMC5883l::getHeading(){
    vec3f scaled = getAxes();

    float headingRad =  atan2(scaled.y, scaled.x);
    headingRad += declination_;

    // Correct for reversed heading
    if(headingRad < 0)
        headingRad += 2*PI;

    // Check for wrap and compensate
    if(headingRad > 2*PI)
        headingRad -= 2*PI;

    // Convert to degrees from radians
    float headingDeg = headingRad * 180/PI;
    int degrees = int(headingDeg);
    int minutes = int(((headingDeg - degrees) * 60));

    return vec2i(degrees, minutes);
}
