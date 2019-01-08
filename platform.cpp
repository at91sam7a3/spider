#include "platform.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

namespace spider {

static const double minimumDistanceStep=10; //TODO requires experiments

const double PI=3.141592654;
const double  rotateRadius=220;

void Platform::Sleep()
{
    for(int i=0;i<6;++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        legs_[i].SetMotorAngle(0,180);

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        legs_[i].SetMotorAngle(1,0);

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        legs_[i].SetMotorAngle(2,0);
    }
}

Platform::Platform()
    :state_(Sleeping)
    ,currentCoordinates(0,0)
    ,moveSpeed(4)
    ,normalizedMovementVector()
    ,currentMovementSpeed(0)
    ,distanceLeft_(0.0)
    ,directionAngle(0.0) //lets say we looking at North
{
    for (int i=0; i<6;++i)
    {
        Leg leg;
        leg.SetLegIndex(i);
        legs_.push_back(leg);
    }
    prepareToGo();
    moving_thread_.reset(new std::thread(&Platform::MovementThread,this));
}

void Platform::GoToPosition(vec2f newCoord)
{
    std::cout<<"We at position "<<currentCoordinates.x<<" , "<<currentCoordinates.y<<std::endl;
    std::cout<<"Starting move to new position "<<newCoord.x<<","<<newCoord.y<<std::endl;
    vec2f newMove;
    newMove.x = newCoord.x - currentCoordinates.x;
    newMove.y = newCoord.y - currentCoordinates.y;
    distanceLeft_ = sqrt( newMove.x * newMove.x + newMove.y  * newMove.y  );
    normalizedMovementVector.x=newMove.x / distanceLeft_;
    normalizedMovementVector.y=newMove.y / distanceLeft_;
    normalizedMovementVector.rotate(-directionAngle); //if we look east and have to go north then locally we go west
    std::cout<<"changing state to going"<<std::endl;
    std::cout<<"movement vector x,y="<<normalizedMovementVector.x<<" , "<<normalizedMovementVector.y<<std::endl;
    std::cout<<"distance "<<distanceLeft_<<std::endl;
    prepareToGo();
    state_ = MovementState::Going;
}

void Platform::Turn(float degrees)
{
    desiredAngle_ = degrees;
    prepareToTurn();
    state_ = MovementState::Rotating;
}

void Platform::SetBodyHeight(float height)
{
    for (size_t i=0; i<6;++i)
    {
        legs_[i].bodyHeight_=height;
        legs_[i].RecalcAngles();
    }
    bodyHeight_=height;
}

float Platform::GetBodyHeight()
{
    return bodyHeight_;
}

void Platform::procedureGo()
{
    bool anyLegInAir = false; //so, only one leg allowed to be in air at once
    //  bool anyLegNotInCenter=false;
    for(Leg& currentLeg : legs_)
    {
        // if(!currentLeg.IsInCenter()) anyLegNotInCenter = true; //TODO maybe optimization is needed here
        if(currentLeg.leg_position == Leg::in_air)
        {
            anyLegInAir = true;
            if(currentLeg.IsInCenter())
                currentLeg.MoveLegDown();
            else {
                currentLeg.MoveLegToCenter();
            }
        }
        else //leg on a ground
        {
            if(distanceLeft_>moveSpeed) //make a full step
            {
                currentLeg.LegAddOffsetInGlobal(normalizedMovementVector.x*moveSpeed,normalizedMovementVector.y*moveSpeed);
            }
            else {
                currentLeg.LegAddOffsetInGlobal(normalizedMovementVector.x*distanceLeft_,normalizedMovementVector.y*distanceLeft_);
            }
        }
    }
    if(distanceLeft_>moveSpeed) //make a full step
    {
        distanceLeft_-=moveSpeed;
        currentCoordinates.x+=normalizedMovementVector.x*moveSpeed;
        currentCoordinates.y+=normalizedMovementVector.y*moveSpeed;
    }
    else {
        currentCoordinates.x+=normalizedMovementVector.x*distanceLeft_;
        currentCoordinates.y+=normalizedMovementVector.y*distanceLeft_;
        std::cout<<"We came to position "<<currentCoordinates.x<<" , "<<currentCoordinates.y<<std::endl;
        distanceLeft_=0;
        state_ = MovementState::Idle;
        prepareToGo();
        return;
    }

    std::cout<<"distance left ="<<distanceLeft_<<std::endl;;
    if(!anyLegInAir)//all 6 legs on the ground
    {
        double maxDist=0;
        int legToRaise=-1;
        for(Leg& currentLeg : legs_)
        {
            double curDist = currentLeg.GetDistanceFromCenter();
            if(curDist>maxDist)
            {
                maxDist=curDist;
                legToRaise=currentLeg.GetLegIndex();
            }
        }
        // std::cout<<"max dist # "<<legToRaise<<std::endl;
        if(maxDist>=minimumDistanceStep){ //Dont make steps if it not really needed
            std::cout<<"Move up leg # "<<legToRaise<<std::endl;
            if(legToRaise!=-1){ // I think this check not needed, but let it be
                legs_[legToRaise].MoveLegUp();
            }
        }
    }
    for(Leg& currentLeg : legs_)
    {
        currentLeg.RecalcAngles();//AMEN
    }
}

void Platform::procedureTurn()
{
    for(size_t leg=0;leg<6;++leg)
    {
        double legAngle=legs_[leg].GetLegDirectionInGlobalCoordinates();

        LegCoodinates gc(cos( legAngle/180*PI ) *(rotateRadius)
                         ,-sin( legAngle/180*PI ) *(rotateRadius),0  );
        LegCoodinates lc = legs_[leg].GlobalToLocal(gc);
        legs_[leg].SetXY(lc.x,lc.y);
    }
}

void Platform::prepareToGo()
{
    for(size_t i=0;i<6;++i){
        {
            if(!legs_[i].IsInCenter()){
                legs_[i].MoveLegUp();
                legs_[i].MoveLegToCenter();
                legs_[i].RecalcAngles();
                MovementDelay();
            }
            legs_[i].MoveLegDown();
            legs_[i].RecalcAngles();
            MovementDelay();
            MovementDelay();

        }
    }
}

void Platform::prepareToTurn()
{

    for(size_t leg=0;leg<6;++leg)
    {
        double legAngle=legs_[leg].GetLegDirectionInGlobalCoordinates();
        LegAngles_tmp[leg]=legAngle;
        LegCoodinates gc(cos( legAngle/180*PI ) *(rotateRadius)
                         ,-sin( legAngle/180*PI ) *(rotateRadius),0  );

        LegCoodinates lc = legs_[leg].GlobalToLocal(gc);
        legs_[leg].MoveLegUp();
        legs_[leg].SetXY(lc.x,lc.y);
        legs_[leg].RecalcAngles();
        MovementDelay();
        legs_[leg].MoveLegDown();
        legs_[leg].RecalcAngles();
        MovementDelay();
    }
}

void Platform::MovementThread()
{
    std::cout<<"Start moving thread"<<std::endl;
    // GoToPosition(vec2f(300,300));
    //  Sleep();
    bool move_in_progress=true;
    do
    {
        //So, here we are
        //We need to think, have we make some movement?
        switch(state_)
        {
        case spider::Sleeping:
            //Clear that it`s power off, and nothing to do here
            break;
        case spider::Idle:

            break;
        case spider::Going:
            procedureGo();
            break;
        case spider::Rotating:
            //TODO not implementer yet
            procedureTurn();
            break;

        }

        //sleep if nothing to do
        MovementDelay();
    }
    while(move_in_progress);
}

void Platform::MovementDelay()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
}