#include "platform.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include "telemetrymanager.h"

namespace spider {

static const double minimumDistanceStep=10; //TODO requires experiments

const double PI=3.141592654;

Platform *Platform::GetInstance()
{
    static Platform platform;
    return &platform;
}

//place legs in compact position for transportation
void Platform::Sleep()
{
    for(unsigned int i=0;i<6;++i)
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
    ,normalizedMovementVector(0,1)
    ,currentMovementSpeed(4)
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
  //  Sleep();
    moving_thread_.reset(new std::thread(&Platform::MovementThread,this));
}



void Platform::GoToOffset(vec2f newCoord)
{
    std::cout<<"We at position "<<currentCoordinates<<std::endl;
    std::cout<<"Starting move to new position "<<newCoord<<std::endl;
    vec2f newMove;
    newMove = newCoord - currentCoordinates;

    distanceLeft_ = sqrt( newMove.x * newMove.x + newMove.y  * newMove.y  );
    if(distanceLeft_<0.001){
        std::cout<<"Moving to current position not allowed, skipping"<<std::endl;
        normalizedMovementVector.x=0;
        normalizedMovementVector.y=0;
        return;
    }
    normalizedMovementVector.x=newMove.x / distanceLeft_;
    normalizedMovementVector.y=newMove.y / distanceLeft_;
    normalizedMovementVector.rotate(-directionAngle); //if we look east and have to go north then locally we go west
    std::cout<<"changing state to going"<<std::endl;
    std::cout<<"movement vector x,y="<<normalizedMovementVector.x<<" , "<<normalizedMovementVector.y<<std::endl;
    std::cout<<"distance "<<distanceLeft_<<std::endl;
    prepareToGo();
    state_ = MovementState::Going;
}

void Platform::MoveForward(float distance)
{
    vec2f newMove;
    newMove.x = normalizedMovementVector.x *distance + currentCoordinates.x;
    newMove.y = normalizedMovementVector.y*distance + currentCoordinates.y;

    GoToCoordinates(newMove);
}



void Platform::GoToCoordinates(vec2f newCoord)
{
    std::cout<<"We at position "<<currentCoordinates.x<<" , "<<currentCoordinates.y<<std::endl;
    std::cout<<"Starting move to new position "<<newCoord.x<<","<<newCoord.y<<std::endl;
    vec2f newMove;
    newMove.x = newCoord.x - currentCoordinates.x;
    newMove.y = newCoord.y - currentCoordinates.y;
    distanceLeft_ = sqrt( newMove.x * newMove.x + newMove.y  * newMove.y  );
    if(distanceLeft_<0.001){
        std::cout<<"Moving to current position not allowed, skipping"<<std::endl;
        normalizedMovementVector.x=0;
        normalizedMovementVector.y=0;
        return;
    }
    normalizedMovementVector.x=newMove.x / distanceLeft_;
    normalizedMovementVector.y=newMove.y / distanceLeft_;
    //normalizedMovementVector.rotate(); //if we look east and have to go north then locally we go west
    std::cout<<"changing state to going"<<std::endl;
    std::cout<<"movement vector x,y="<<normalizedMovementVector.x<<" , "<<normalizedMovementVector.y<<std::endl;
    std::cout<<"distance "<<distanceLeft_<<std::endl;
    prepareToGo();
    state_ = MovementState::Going;
}


void Platform::Turn(float degrees)
{
    std::cout <<"!!!Executing turn command"<<std::endl;
    desiredAngle_ = degrees;
    prepareToGo();
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

void Platform::movingEnd()
{
    for(size_t i=0;i<6;++i){
        {
            legs_[i].MoveLegDown();
            legs_[i].RecalcAngles();
        }
    }
}

void Platform::procedureGo()
{
    bool anyLegInAir = false; //so, only one leg allowed to be in air at once
    //  bool anyLegNotInCenter=false;
    for(Leg& currentLeg : legs_)
    {
        if(currentLeg.leg_position != Leg::on_ground)
        {
            anyLegInAir = true;
            currentLeg.ProcessLegMovingInAir();
        }
        else //leg on a ground
        {
            if(distanceLeft_>currentMovementSpeed) //make a full step
            {
                currentLeg.LegAddOffsetInGlobal(normalizedMovementVector.x*currentMovementSpeed,normalizedMovementVector.y*currentMovementSpeed);
            }
            else {
                currentLeg.LegAddOffsetInGlobal(normalizedMovementVector.x*distanceLeft_,normalizedMovementVector.y*distanceLeft_);
            }
        }
    }
    if(distanceLeft_>currentMovementSpeed) //make a full step
    {
        distanceLeft_-=currentMovementSpeed;
        currentCoordinates.x+=normalizedMovementVector.x*currentMovementSpeed;
        currentCoordinates.y+=normalizedMovementVector.y*currentMovementSpeed;
    }
    else {
        currentCoordinates.x+=normalizedMovementVector.x*distanceLeft_;
        currentCoordinates.y+=normalizedMovementVector.y*distanceLeft_;
        std::cout<<"We came to position "<<currentCoordinates.x<<" , "<<currentCoordinates.y<<std::endl;
        distanceLeft_=0;
        state_ = MovementState::Idle;
        movingEnd();
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

                vec2f newPoint(legs_[legToRaise].GetCenterVec());
                std::cout<<"Center is "<<newPoint.x<<" , "<<newPoint.y<<std::endl;
                vec2f tmpOffsetVec=normalizedMovementVector*(maxDist/2);
                newPoint=newPoint+tmpOffsetVec;
                std::cout<<"new point "<<newPoint.x<<" , "<<newPoint.y<<std::endl;
                legs_[legToRaise].MoveLegUp(newPoint);
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
    double rotationSpeed=1;
    bool anyLegInAir = false; //so, only one leg allowed to be in air at once
    double rot=0;
    //here we check where to turn
    if(directionAngle > desiredAngle_)
        rot=-rotationSpeed;
    else
        rot=rotationSpeed;


    std::cout<<"Procedure turn____________________"<<std::endl;

    std::cout<<" angle "<<directionAngle<<std::endl;
    std::cout<<"desiredAngle_ angle "<<desiredAngle_<<std::endl;
    std::cout<<"rot"<<rot<<std::endl;

    //  bool anyLegNotInCenter=false;
    for(Leg& currentLeg : legs_)
    {
        if(currentLeg.leg_position != Leg::on_ground)
        {
            anyLegInAir = true;
            currentLeg.ProcessLegMovingInAir();
        }
        else //leg on a ground
        {
            currentLeg.TurnLegWithGlobalCoord(rot);
            currentLeg.RecalcAngles();
        }
    }
    directionAngle+=rot;

    if(fabs(directionAngle-desiredAngle_)<rotationSpeed)
    {
        state_ = MovementState::Idle;
        movingEnd();
        return;
    }


    if(!anyLegInAir)//all 6 legs on the ground
    {
        double maxDist=0;
        int legToRaise=-1;
        for(Leg& currentLeg : legs_)
        {
            double curDist = (currentLeg.GetDistanceFromCenter());
              std::cout<<"cur dist # "<<curDist<<std::endl;
            if(curDist>maxDist)
            {
                maxDist=curDist;
                legToRaise=currentLeg.GetLegIndex();
            }
        }
         std::cout<<"max dist # "<<legToRaise<<std::endl;
        if(maxDist>=45){ //Dont make steps if it not really needed
            std::cout<<"Move up leg # "<<legToRaise<<std::endl;
            if(legToRaise!=-1){ // I think this check not needed, but let it be

                vec2f newPoint(legs_[legToRaise].GetCenterVec());
                std::cout<<"Center is "<<newPoint.x<<" , "<<newPoint.y<<std::endl;
                /*vec2f tmpOffsetVec=normalizedMovementVector*(maxDist/2);
                newPoint=newPoint+tmpOffsetVec;
                std::cout<<"new point "<<newPoint.x<<" , "<<newPoint.y<<std::endl;*/
                legs_[legToRaise].MoveLegUp(newPoint);
            }
        }
    }
    for(Leg& currentLeg : legs_)
    {
        currentLeg.RecalcAngles();//AMEN
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

    /* for(size_t leg=0;leg<6;++leg)
    {
        double legAngle=legs_[leg].GetLegDirectionInGlobalCoordinates();
        LegAngles_tmp[leg]=legAngle;
        LegCoodinates gc(cos( legAngle/180*PI ) *(rotateRadius)
                         ,-sin( legAngle/180*PI ) *(rotateRadius),0  );

        LegCoodinates lc = legs_[leg].GlobalToLocal(gc);
        legs_[leg].MoveLegUp();
        legs_[leg].SetLocalXY(lc.x,lc.y);
        legs_[leg].RecalcAngles();
        MovementDelay();
        legs_[leg].MoveLegDown();
        legs_[leg].RecalcAngles();
        MovementDelay();
    }*/
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
             TelemetryManager::GetInstance()->setValue("X Coord",currentCoordinates.x);
              TelemetryManager::GetInstance()->setValue("Y Coord",currentCoordinates.y);
            break;
        case spider::Rotating:
            //TODO not implementer yet
            procedureTurn();
             TelemetryManager::GetInstance()->setValue("Direction",directionAngle);
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
