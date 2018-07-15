#ifndef I2CSUPERVISOR_H
#define I2CSUPERVISOR_H


class i2cSupervisor
{
public:
    i2cSupervisor();
    //TODO add mutex
    static int I2CRead           (int fd) ;
    static int I2CReadReg8       (int fd, int reg) ;
    static int I2CReadReg16      (int fd, int reg) ;

    static int I2CWrite          (int fd, int data) ;
    static int I2CWriteReg8      (int fd, int reg, int data) ;
    static int I2CWriteReg16     (int fd, int reg, int data) ;

    static int I2CSetupInterface (const char *device, int devId) ;
    static int I2CSetup          (const int devId) ;

    static void IICwriteBit(int fd, char reg, char bitNum, char data);

    static void IICwriteBits(int fd,char reg,char bitStart,char length,char data);
    static void IICreadBytes(int fd, int start, int size, char *data);
};

#endif // I2CSUPERVISOR_H
