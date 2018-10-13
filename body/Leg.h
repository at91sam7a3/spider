#ifndef LEG_H
#define LEG_H

#include <string>
#include <vector>

namespace spider
{

enum Legs
{
    RightFront=0,
    RightMiddle,
    RightBack,
    LeftBack,
    LeftMiddle,
    LeftFront
};

struct LegCoodinates
{
    LegCoodinates(float x1,float y1,float h1)
        :x(x1)
        ,y(y1)
        ,height(h1)
    {

    }
    LegCoodinates()
        :x(0)
        ,y(0)
        ,height(0)
    {

    }
    float x;
    float y;
    float height;
};



class Leg
{
public:
    Leg();

    void SetLegIndex(int idx);
    void RecalcAngles();
    void SetXY(float,float);

    void SetLegCoord(LegCoodinates & lc);
    LegCoodinates GetLegCoord();
    //input
    std::string GetStringForServos();
    std::vector<int> GetMotorIndexes();
    // convert global coordinates to local for this leg
    LegCoodinates GlobalToLocal(LegCoodinates &lc);
    // get Leg angle
    float GetLegDirectionInGlobalCoordinates();
    float bodyHeight_;

    void SetMotorAngle(int idx,int angle);
private:
    float xPos_;
    float yPos_;

    float distanceFromGround_; // leg height
    //output, angles in radians
    float angleA_;
    float angleB_;
    float angleC_;
    //setted servos numbers
    std::vector<int> indexes_;
    int legIndex_;
    float angleCOffsetAccordingToLegAttachment_;
};
}
#endif // LEG_H
