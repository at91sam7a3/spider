#include "Leg.h"
#include <cmath>
#include <iostream>
#include "../servomanager.h"



namespace spider {

static const float cLegPart=52;
static const float bLegPart=81;
static const float aLegPart=122;

static const float centerYOffset=0;  //65
static const float rearYOffset=0;  //45
static const float rearXOffset=0;

Leg::Leg()
    :xPos_(0)
    ,yPos_(10)
    ,bodyHeight_(120)
    ,distanceFromGround_(0)
{

}

void Leg::SetLegIndex(int idx)
{
    legIndex_ = idx;
    indexes_.push_back(idx*3);
    indexes_.push_back(idx*3+1);
    indexes_.push_back(idx*3+2);

    if((idx==0) ||(idx==5)) { xPos_=40; yPos_=10;}
    if((idx==2) ||(idx==3)) { xPos_=-40;yPos_=10;}
}

void Leg::RecalcAngles()
{
    if(yPos_==0) yPos_=0.01;
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

    angleA_ = angleA_ *180 / 3.1415;
    angleB_ = angleB_ *180 /3.1415;
    angleC_ = angleC_ *180 /3.1415;
    ServoManager::setAngleF(indexes_[0],180-angleA_);
    ServoManager::setAngleF(indexes_[1],angleB_);
    ServoManager::setAngleF(indexes_[2],angleC_);
    //std::cout<<"angles<< A "<<angleA_<<" B "<<angleB_<<" C "<<angleC_<<std::endl;
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

        res.x= res.x-20;
    }
    return res;
}

float Leg::GetLegDirectionInGlobalCoordinates()
{
    switch(legIndex_)
    {
    case RightMiddle:
        return -90;
        break;

    case LeftMiddle:
        return 90;
        break;

    case RightFront:
        return -30;
        break;

    case RightBack:
        return -150;
        break;

    case LeftFront:
        return 30;
        break;

    case LeftBack:
        return 150;
        break;

    default:

        break;
    }
}
}




