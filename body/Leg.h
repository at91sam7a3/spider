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
    double x;
    double y;
    double height;
};



class Leg
{
public:
    Leg();

    void SetLegIndex(int idx);
    int GetLegIndex();
    void RecalcAngles();
    void SetXY(double,double);
    void LegAddOffsetInGlobal(double,double);

    void SetLegCoord(LegCoodinates & lc);
    LegCoodinates GetLegCoord();
    //input
    std::vector<int> GetMotorIndexes();
    // convert global coordinates to local for this leg
    LegCoodinates GlobalToLocal(LegCoodinates &lc);
    // get Leg angle
    double GetLegDirectionInGlobalCoordinates();
    double bodyHeight_;

    void SetMotorAngle(int idx,int angle);
    enum LegPosition { on_ground=0, in_air=1 } leg_position;
    double GetDistanceFromCenter();
    bool IsInCenter();
    void MoveLegUp();
    void MoveLegDown();
    void MoveLegToCenter();
    // this is needed only for rotating procesure
    float currentLegrotationOffset_;
private:
    volatile double xPos_;
    volatile double yPos_;

    double xCenterPos_;
    double yCenterPos_;

    double distanceFromGround_; // leg height
    //output, angles in radians
    double angleA_;
    double angleB_;
    double angleC_;
    //setted servos numbers
    std::vector<int> indexes_;
    int legIndex_;
    float angleCOffsetAccordingToLegAttachment_;

};
}
#endif // LEG_H
