#include "Leg.h"
#include <cmath>
#include <iostream>
#include "../servomanager.h"



namespace spider {

std::ostream& operator <<(std::ostream& out, const vec2f& vec){
    out<<"vec2f( "<<vec.x<<" , "<<vec.y<<" ) ";
    return out;
}
//leg parts sizes
static const double cLegPart=52;
static const double bLegPart=81;
static const double aLegPart=124;
// phisical coordinates where legs attached on body
static const double centerYOffset=85;  //65
static const double rearYOffset=50;
static const double rearXOffset=86.5;
static const double stepHeight = 80;//How far robot raise a leg on step

Leg::Leg()
    :bodyHeight_(60)
    ,leg_position(on_ground)
    ,currentLegrotationOffset_(0)
    ,xPos_(0)
    ,yPos_(0)
    ,xCenterPos_(0)
    ,yCenterPos_(0)
    ,distanceFromGround_(0)
{

}

void Leg::SetLegIndex(int idx)
{
    legIndex_ = idx;
    indexes_.push_back(idx*3);
    indexes_.push_back(idx*3+1);
    indexes_.push_back(idx*3+2);

    angleCOffsetAccordingToLegAttachment_ = 0;

    if(idx==0) angleCOffsetAccordingToLegAttachment_ = -30;
    if(idx==1) angleCOffsetAccordingToLegAttachment_ = -90;
    if(idx==2) angleCOffsetAccordingToLegAttachment_ = -150;
    if(idx==3) angleCOffsetAccordingToLegAttachment_ = -150;
    if(idx==4) angleCOffsetAccordingToLegAttachment_ = -90;
    if(idx==5) angleCOffsetAccordingToLegAttachment_ = -30;

    if((idx==1) || (idx==4))
        yCenterPos_=160;

    if((idx==0) || (idx==5))
        xCenterPos_=60;
    yCenterPos_=100;

    if((idx==2) || (idx==3))
        xCenterPos_=-60;
    yCenterPos_=100;

}

int Leg::GetLegIndex()
{
    return legIndex_;
}

void Leg::RecalcAngles()
{
    if(yPos_==0.0) yPos_=0.01;
    // angle Gamma (angleC_)
    angleC_ = (atan(xPos_/yPos_));
    double L1 = sqrt( xPos_ * xPos_ + yPos_ * yPos_ );
    double L = sqrt( pow(bodyHeight_,2.0) + pow((L1 - cLegPart),2));
    // angle alpfa
    angleA_ = acos((bodyHeight_-distanceFromGround_)/L)
            + acos(((pow(aLegPart,2)-pow(bLegPart,2)-pow(L,2)))
                   /(-2 * bLegPart * L));
    // angle beta
    angleB_ = acos((pow(L,2)-pow(aLegPart,2)-pow(bLegPart,2))
                   /(-2 * aLegPart * bLegPart));

    //set angles directly to servos

    angleA_ = angleA_ *180 / 3.1415;
    angleB_ = angleB_ *180 /3.1415;
    angleC_ = angleC_ *180 /3.1415;
    ServoManager::setAngleF(indexes_[0], angleA_);
    ServoManager::setAngleF(indexes_[1],180 -angleB_);
    ServoManager::setAngleF(indexes_[2],angleC_-angleCOffsetAccordingToLegAttachment_);
    // std::cout<<"LEG "<<legIndex_<<" angles<< A "<<angleA_<<" B "<<angleB_<<" C "<<angleC_<<std::endl;
}

void Leg::SetLocalXY(double x, double y) //TODO
{
    xPos_ = x;
    yPos_ = y;
    if(legIndex_==0)
    {
        std::cout<<" set LEG0 x,y = "<<x<<","<<y<<std::endl;
    }
}

void Leg::LegAddOffsetInGlobal(double xoffset, double yoffset)
{
    xPos_-=xoffset;
    if(legIndex_<3)
    {
        yPos_+=yoffset;
    }
    else {
        yPos_-=yoffset;
    }
}

void Leg::SetLegCoord(LegCoodinates &lc)
{
    SetLocalXY(lc.x,lc.y);
    distanceFromGround_ = lc.height;
    //RecalcAngles();
}

LegCoodinates Leg::GetLegCoord()
{
    if(legIndex_==0)
    {
        std::cout<<" get LEG0 x,y = "<<xPos_<<","<<yPos_<<std::endl;
    }
    LegCoodinates lc(xPos_,yPos_,distanceFromGround_);
    return lc;
}

std::vector<int> Leg::GetMotorIndexes()
{
    return indexes_;
}


// X axis looks front
// Y axit looks left
vec2f Leg::GlobalToLocal(vec2f &lc)
{
    vec2f res;
    switch (legIndex_) {

    case RightMiddle:
        res.x = lc.x;
        res.y = lc.y-centerYOffset;
        break;
    case LeftMiddle:
        res.x = lc.x;
        res.y = -lc.y - centerYOffset;
        break;

    case RightFront:
        res.x = lc.x - rearXOffset;
        res.y = lc.y - rearYOffset;
        break;
    case RightBack:
        res.x = lc.x + rearXOffset;
        res.y = lc.y - rearYOffset;
        break;

    case LeftFront:
        res.x = lc.x - rearXOffset;
        res.y = -lc.y - rearYOffset;
        break;
    case LeftBack:
        res.x = lc.x + rearXOffset;
        res.y = -lc.y - rearYOffset;
        break;

    default:
        break;
    }
    return res;
};

double Leg::GetLegDirectionInGlobalCoordinates()
{
    switch(legIndex_)
    {
    case RightMiddle:
        return -90;

    case LeftMiddle:
        return 90;

    case RightFront:
        return -60;

    case RightBack:
        return -120;

    case LeftFront:
        return 60;

    case LeftBack:
        return 120;

    default:
        throw std::exception();
        break;
    }
}

void Leg::SetMotorAngle(int idx, int angle)
{
    switch(idx)
    {
    case 0:
        ServoManager::setAngleF(indexes_[0], angle);
        break;
    case 1:
        ServoManager::setAngleF(indexes_[1],180 -angle);
        break;
    case 2:
        ServoManager::setAngleF(indexes_[2],angle-angleCOffsetAccordingToLegAttachment_);
        break;
    }
}

double Leg::GetDistanceFromCenter()
{
    double xDist=fabs(xPos_-xCenterPos_);
    double yDist=fabs(yPos_-yCenterPos_);
    return sqrt( xDist * xDist + yDist * yDist );
}

bool Leg::IsInCenter()
{
    if((fabs(xPos_-xCenterPos_) < 0.001) && (fabs(yPos_-yCenterPos_)<0.001)) return true;
    return false;
}

void Leg::MoveLegUp()
{
    distanceFromGround_ = stepHeight;
    leg_position = moving_up;
}

void Leg::MoveLegDown()
{
    distanceFromGround_ = 0;
    leg_position = on_ground;
}

void Leg::MoveLegToCenter()
{
    xPos_=xCenterPos_;
    yPos_=yCenterPos_;
}

void Leg::MoveLegUp(vec2f newPositionOnGround)
{
    if(leg_position != on_ground)
        throw(std::runtime_error("try to move up leg that already in air"));
    newPositionOnGround_ = newPositionOnGround;
    distanceFromGround_ = stepHeight;
    leg_position=moving_up;
}

void Leg::ProcessLegMovingInAir()
{
    if(leg_position==moving_up){
        leg_position=moving_to_target;
        SetLocalXY(newPositionOnGround_.x,newPositionOnGround_.y);
        return;
    }
    if(leg_position==moving_to_target){
        leg_position=on_ground;
        distanceFromGround_=0;
    }
}

vec2f Leg::GetCenterVec()
{
    return vec2f(xCenterPos_,yCenterPos_ );
}

double Leg::GetLegLocalZAngle()
{
    return angleC_;
}

void Leg::TurnLegWithGlobalCoord(double offset)
{
    if(legIndex_ == 0){
        std::cout<<"Tur leg on "<<offset<<" degrees"<<std::endl;
           std::cout<<"Leg 0 local pos "<<xPos_<<" , "<<yPos_<<" "<<std::endl;
    }

    vec2f currentGlobalPos = GetLegGlobalCoord();
    if(legIndex_ == 0){
        std::cout<<"Leg 0 global pos "<<currentGlobalPos.x<<" , "<<currentGlobalPos.y<<" "<<std::endl;
    }
    currentGlobalPos.rotate(offset);
    if(legIndex_ == 0){
        std::cout<<"Leg 0 global pos "<<currentGlobalPos.x<<" , "<<currentGlobalPos.y<<" after rotate "<<std::endl;
    }
    vec2f lc = GlobalToLocal(currentGlobalPos);
    SetLocalXY(lc.x,lc.y);
    if(legIndex_ == 0){
           std::cout<<"Leg 0 new local pos "<<xPos_<<" , "<<yPos_<<" "<<std::endl;
    }
}

vec2f Leg::GetLegGlobalCoord()
{
    vec2f res;

    switch (legIndex_) {

    case RightMiddle:
        res.x = xPos_;
        res.y = yPos_+ centerYOffset;
        break;
    case LeftMiddle:
        res.x = xPos_;
        res.y = -yPos_ - centerYOffset;
        break;

    case RightFront:
        res.x = xPos_ + rearXOffset;
        res.y = yPos_ + rearYOffset;
        break;
    case RightBack:
        res.x = xPos_ - rearXOffset;
        res.y = yPos_ + rearYOffset;
        break;

    case LeftFront:
        res.x = xPos_ + rearXOffset;
        res.y = -yPos_ - rearYOffset;
        break;
    case LeftBack:
        res.x = xPos_ - rearXOffset;
        res.y = -yPos_ - rearYOffset;
        break;

    default:
        break;
    }
    return res;
}

}




