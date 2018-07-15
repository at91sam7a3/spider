#ifndef HMC5883L_H
#define HMC5883L_H

struct vec3f
{
    float x;
    float y;
    float z;
};

struct vec3i
{
    int x;
    int y;
    int z;
};

struct vec2i
{
    vec2i(int x1,int y1):
        x(x1)
      ,y(y1)
    {
        ;
    }

    int x;
    int y;
};

class HMC5883l
{

public:
    HMC5883l(char addr = 0x1e,float gauss = 1.3);

    void setScale(float gauss);

    void setOption(char reg, char data);

    void setContinuousMode();

    void setDeclination(int degree = 0,int min = 0);

    vec3f getAxes();

    //Returns heading in degrees and minutes
    vec2i getHeading();

private:
    float declination_;
    int declinationMin_;
    int declinationDeg_;
    char address_;
    int i2c;
    float scale;
};
























        #endif // HMC5883L_H
