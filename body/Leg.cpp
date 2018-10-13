#include "Leg.h"
#include <cmath>
#include <iostream>
#include "../servomanager.h"



namespace spider {
//leg parts sizes
static const float cLegPart=52;
static const float bLegPart=81;
static const float aLegPart=124;
// phisical coordinates where legs attached on body
static const float centerYOffset=93.5;  //65
static const float rearYOffset=57.5;
static const float rearXOffset=99;

Leg::Leg()
    :bodyHeight_(120)
    ,xPos_(0)
    ,yPos_(0)
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
    // if((idx==0) ||(idx==5)) { xPos_=10; yPos_=10;}
    // if((idx==2) ||(idx==3)) { xPos_=-10;yPos_=10;}
/*    if(idx==0) angleCOffsetAccordingToLegAttachment_ = -60;
    if(idx==1) angleCOffsetAccordingToLegAttachment_ = -90;
    if(idx==2) angleCOffsetAccordingToLegAttachment_ = -120;
    if(idx==3) angleCOffsetAccordingToLegAttachment_ = -120;
    if(idx==4) angleCOffsetAccordingToLegAttachment_ = -90;
    if(idx==5) angleCOffsetAccordingToLegAttachment_ = -60;  */


        if(idx==0) angleCOffsetAccordingToLegAttachment_ = -30;
        if(idx==1) angleCOffsetAccordingToLegAttachment_ = -90;
        if(idx==2) angleCOffsetAccordingToLegAttachment_ = -150;
        if(idx==3) angleCOffsetAccordingToLegAttachment_ = -150;
        if(idx==4) angleCOffsetAccordingToLegAttachment_ = -90;
        if(idx==5) angleCOffsetAccordingToLegAttachment_ = -30;

}

void Leg::RecalcAngles()
{
    if(yPos_==0) yPos_=0.01f;
    // angle Gamma (angleC_)
    angleC_ = atan(xPos_/yPos_);
    float L1 = sqrt( xPos_ * xPos_ + yPos_ * yPos_ );
    float L = sqrt( pow(bodyHeight_,2.0) + pow((L1 - cLegPart),2));
    // angle alpfa
    angleA_ = acos((bodyHeight_-distanceFromGround_)/L)
            + acos(((pow(aLegPart,2)-pow(bLegPart,2)-pow(L,2)))
                   /(-2 * bLegPart * L));
    // angle beta
    angleB_ = acos((pow(L,2)-pow(aLegPart,2)-pow(bLegPart,2))
                   /(-2 * aLegPart * bLegPart));

    //set angles directly to servos

    angleA_ = angleA_ *180 / 3.1415f;
    angleB_ = angleB_ *180 /3.1415f;
    angleC_ = angleC_ *180 /3.1415f;
    ServoManager::setAngleF(indexes_[0], angleA_);
    ServoManager::setAngleF(indexes_[1],180 -angleB_);
    ServoManager::setAngleF(indexes_[2],angleC_-angleCOffsetAccordingToLegAttachment_);
    std::cout<<"LEG "<<legIndex_<<" angles<< A "<<angleA_<<" B "<<angleB_<<" C "<<angleC_<<std::endl;
}

void Leg::SetXY(float x, float y) //TODO
{
    xPos_ = x;
    yPos_ = y;
}

void Leg::SetLegCoord(LegCoodinates &lc)
{
    SetXY(lc.x,lc.y);
    distanceFromGround_ = lc.height;
    RecalcAngles();
    //std::cout <<" Leg "<<legIndex_<<" set "<<lc.x<<" , "<<lc.y<<" , "<<lc.height<<std::endl;
    //std::cout <<" body height "<<bodyHeight_<<std::endl;
}

LegCoodinates Leg::GetLegCoord()
{
    LegCoodinates lc(xPos_,yPos_,distanceFromGround_);
    return lc;
}

std::string Leg::GetStringForServos()
{
    return std::string();
}

std::vector<int> Leg::GetMotorIndexes()
{
    return indexes_;
}


// X axis looks front
// Y axit looks left
LegCoodinates Leg::GlobalToLocal(LegCoodinates &lc)
{
    LegCoodinates res;
    res.height = lc.height;
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
}

float Leg::GetLegDirectionInGlobalCoordinates()
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
}




