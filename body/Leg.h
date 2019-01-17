#ifndef LEG_H
#define LEG_H
#include <math.h>
#include <string>
#include <vector>

namespace spider
{

struct vec2f
{
    constexpr static const double PI=3.141592654;
    vec2f()
        :x(0)
        ,y(0){}
    vec2f(double x1,double y1)
        :x(x1)
        ,y(y1){}

    double x;
    double y;

    void rotate(double angle)
    {
        double tmpAngle=angle*PI/180.0;
        double tmpx=(cos(tmpAngle)*x)-(sin(tmpAngle)*y);
        double tmpy=(sin(tmpAngle)*x)-(cos(tmpAngle)*y);
        x=tmpx;
        y=tmpy;
    }
    vec2f operator+(const vec2f& sum){
        return vec2f(x+sum.x, y+sum.y);
    }
    vec2f operator*(const double size){
        return vec2f(x*size,y*size);
    }
};

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
    LegCoodinates(double x1,double y1,double h1)
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
    enum LegPosition { on_ground=0, moving_up, moving_to_target, moving_down} leg_position;
    double GetDistanceFromCenter();
    bool IsInCenter();
    void MoveLegUp();
    void MoveLegDown();
    void MoveLegToCenter();
    void MoveLegUp(vec2f newPositionOnGround);
    void ProcessLegMovingInAir();
    // this is needed only for rotating procesure
    float currentLegrotationOffset_;
    vec2f GetCenterVec();
private:
    volatile double xPos_;
    volatile double yPos_;

    double xCenterPos_;
    double yCenterPos_;

    vec2f newPositionOnGround_;

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
