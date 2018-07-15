#ifndef MPU6050SIMPLE_H
#define MPU6050SIMPLE_H


class mpu6050simple
{
public:
    mpu6050simple(int addr);

    void getRotation(float& x, float& y);


private:
    int address_;
    int i2c;

    float dist(float x, float y);
    float get_y_rotation( float x,float y, float z);
    float get_x_rotation(float x,float y, float z);

    int read_word(char adr);

    int read_word_2c(char adr);

};

#endif // MPU6050SIMPLE_H
