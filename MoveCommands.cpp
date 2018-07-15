#include "MoveCommands.h"
#include <algorithm>
#include <thread>
#include <chrono>
#include <iostream>
#include <math.h>
#define DEBUG_MOVE

#define PI 3.1415926

namespace spider {

MoveCommands* MoveCommands::m_singlton = 0;

MoveCommands* MoveCommands::GetInstance()
{
    if(!m_singlton)
        m_singlton = new MoveCommands();
    return m_singlton;
}

void MoveCommands::MoveForward(float distance)
{

    std::cout<<"Preparing to move"<<std::endl;
    PrepareToMove(Forward);
    std::cout<<"Prepared"<<std::endl;


    if(distance>0)
        PlayAnimation(Forward,distance);
    else
        PlayAnimationReverse(Forward,-distance);

    PrepareToMove(Stop);
}



void MoveCommands::Rotate(float angle)
{

    std::cout<<"Preparing to Rotate"<<std::endl;
    PrepareToMove(TurnLeft);
    std::cout<<"Prepared"<<std::endl;

    if(angle>0)
        PlayAnimation(TurnLeft,angle);
    else
        PlayAnimationReverse(TurnLeft,-angle);
    PrepareToMove(Stop);

}

void MoveCommands::Halt()
{
    PrepareToMove(Stop);
}



void MoveCommands::SetLegPos(int legIdx, float xpos, float ypos, float zpos)
{
    LegCoodinates lc(xpos,ypos,zpos);
    Hull::GetInstance()->legs_[legIdx].SetLegCoord(lc);
}

LegCoodinates MoveCommands::GetLegPos(int legIdx)
{
    return Hull::GetInstance()->legs_[legIdx].GetLegCoord();
}

void MoveCommands::StandUp(int height_)
{
    for(int height=Hull::GetInstance()->GetBodyHeight();height<height_;height=height+4)
    {
        Hull::GetInstance()->SetBodyHeight(height);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }

}

void MoveCommands::SitDown()
{

    for(int height=Hull::GetInstance()->GetBodyHeight();height>100;height=height-4)
    {
        Hull::GetInstance()->SetBodyHeight(height);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }
}

MoveCommands::MoveCommands()
{
    stepSize=64;
    stepWidth=30;
    stepSize2=64;
    stepHeight=60;

    rotateRadius=120;
    stepAngleSize=15;
    stepAngleSize2=15;


    std::cout <<"Generating stop animation"<<std::endl;
    GenerateAnimationStop();
    std::cout <<"Generating move animation"<<std::endl;
    GenerateAnimationMoveForward();

    std::cout <<"Animations ready"<<std::endl;
    // TODO add all another animations here


    ApplyOffsetsToWalkAnimations();
    std::cout <<"Generating TurnLeft animation"<<std::endl;
    GenerateAnimationTurnLeft();
    std::cout<<"Animations loaded: "<<m_animations.size()<<std::endl;
}

void MoveCommands::PrepareToMove(Actions action, bool direct)
{
    PrecalculatedAnimation * anim = &m_animations[action];



    for(int i=0;i<6;++i)
    {
        // move leg up
        LegCoodinates lc =  Hull::GetInstance()->legs_[i].GetLegCoord();
        lc.height = stepHeight;
        Hull::GetInstance()->legs_[i].SetLegCoord(lc);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        // then move leg to current position
        lc = *(anim->frames[i].begin());
        lc.height = stepHeight;
        Hull::GetInstance()->legs_[i].SetLegCoord(lc);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        // move leg down
        lc.height = 0;
        Hull::GetInstance()->legs_[i].SetLegCoord(lc);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}



void MoveCommands::GenerateAnimationStop()
{
    PrecalculatedAnimation precAnim;
    precAnim.totalFrames=1;
    // not need to animate, set interval to 0
    precAnim.frameDelay=0;
    precAnim.totalFrames = 1;
    //LegFramesList stopList;
    LegCoodinates pos(0,stepWidth,0);
    // set the same position for each leg
    for(int i=0;i<6;++i){
        precAnim.frames[i].push_back(pos);

    }

    m_animations.push_back(precAnim);
}



void MoveCommands::GenerateAnimationMoveForward()
{

    PrecalculatedAnimation walkAnimation;
    // *..*..*..*..*..*
    // NnnNnnNnnNnnNnnN  nn
    // NnnNnnNn(S)nNnnNnnN  nn
    float nSize=stepSize/16;//TODO
    LegCoodinates pos(nSize * 8 , stepWidth , 0);
    walkAnimation.frames[0].push_back(pos);

    for(int i=0;i<15;++i)
    {
        pos.x-=nSize;
        walkAnimation.frames[0].push_back(pos);
    }

    pos.height=stepHeight;
    pos.x=-stepSize2/2;
    walkAnimation.frames[0].push_back(pos);
    pos.x=stepSize2/2;
    walkAnimation.frames[0].push_back(pos);

    // here first vector is correct
    for(int i=1;i<6;++i)
    { // from 1 to 5 , 0 is done
        walkAnimation.frames[i] = walkAnimation.frames[i-1];
        std::rotate(walkAnimation.frames[i].begin(),walkAnimation.frames[i].begin()+3,walkAnimation.frames[i].end());
    }

    walkAnimation.frameDelay=50;
    walkAnimation.totalFrames=18;
    m_animations.push_back(walkAnimation);
}

//this is alternative variant
/*
void MoveCommands::GenerateAnimationMoveForward()
{

    PrecalculatedAnimation walkAnimation;
    // *..*..*..*..*..*
    // NnnNnnNnnNnnNnnN  nn
    // NnnNnnNn(S)nNnnNnnN  nn
    float nSize=stepSize/16;//TODO
    LegCoodinates pos(nSize * 8 , stepWidth , 0);
    walkAnimation.frames[0].push_back(pos);

    for(int i=0;i<15;++i)
    {
        pos.x-=nSize;
        walkAnimation.frames[0].push_back(pos);
    }

    pos.height=stepHeight;
    pos.x=-stepSize2/2;
    walkAnimation.frames[0].push_back(pos);
    pos.x=stepSize2/2;
    walkAnimation.frames[0].push_back(pos);

    // here first vector is correct
    for(int i=1;i<3;++i)
    { // from 1 to 5 , 0 is done
        walkAnimation.frames[i] = walkAnimation.frames[i-1];
        std::rotate(walkAnimation.frames[i].begin(),walkAnimation.frames[i].begin()+6,walkAnimation.frames[i].end());
    }

    walkAnimation.frames[3] = walkAnimation.frames[2];
    walkAnimation.frames[4] = walkAnimation.frames[1];
    walkAnimation.frames[5] = walkAnimation.frames[0];

    walkAnimation.frameDelay=50;
    walkAnimation.totalFrames=18;
    m_animations.push_back(walkAnimation);
}
*/

void MoveCommands::GenerateAnimationTurnLeft()
{
    std::cout<<"GenerateAnimationTurnLeft"<<std::endl;
    PrecalculatedAnimation rotateAnimation;
    float nSize=stepAngleSize/16;//TODO

    for(int leg=0;leg<6;++leg)
    {
        std::cout<<"Leg #"<<leg<<std::endl;
        float legAngle=Hull::GetInstance()->legs_[leg].GetLegDirectionInGlobalCoordinates();
        //here we have some angle
        std::cout<<"Leg angle "<<legAngle<<std::endl;
        legAngle+=nSize*8;
        std::cout<<"Leg angle` "<<legAngle<<std::endl;
        int radTweak= ((leg==1)||(leg==4))?0:20;
        for(int i=0;i<16;++i)
        {

            LegCoodinates gc(cos( legAngle/180*PI ) *(rotateRadius+radTweak)
                             ,-sin( legAngle/180*PI ) *(rotateRadius+radTweak),0  );

            LegCoodinates lc = Hull::GetInstance()->legs_[leg].GlobalToLocal(gc);
            rotateAnimation.frames[leg].push_back(lc);
            legAngle-=nSize;
        }
        legAngle=Hull::GetInstance()->legs_[leg].GetLegDirectionInGlobalCoordinates();
        {
            LegCoodinates gc(cos( (legAngle - stepAngleSize2/2) /180*PI ) *(rotateRadius+radTweak),
                             -sin( (legAngle - stepAngleSize2/2)/180*PI ) *(rotateRadius+radTweak),stepHeight  );
            LegCoodinates lc = Hull::GetInstance()->legs_[leg].GlobalToLocal(gc);
            rotateAnimation.frames[leg].push_back(lc);
        }
        {
            LegCoodinates gc(cos( (legAngle + stepAngleSize2/2) /180*PI ) *(rotateRadius+radTweak),
                             -sin( (legAngle + stepAngleSize2/2)/180*PI ) *(rotateRadius+radTweak),stepHeight  );
            LegCoodinates lc = Hull::GetInstance()->legs_[leg].GlobalToLocal(gc);
            rotateAnimation.frames[leg].push_back(lc);
        }
        if(leg>0)
            std::rotate(rotateAnimation.frames[leg].begin(),rotateAnimation.frames[leg].begin()+(3*leg)
                        ,rotateAnimation.frames[leg].end());
    }
    rotateAnimation.totalFrames=18;
    m_animations.push_back(rotateAnimation);
}

void MoveCommands::ApplyOffsetsToWalkAnimations()
{
    for(int animationIdx = 0; animationIdx < m_animations.size() ; animationIdx++){
        int frames=m_animations[animationIdx].totalFrames;

        // set the same position for each leg
        for(int i=0;i<6;++i){

            if((i==1) || (i==4)){
                for(int j=0;j<m_animations[animationIdx].frames[i].size();j++)
                {
                    m_animations[animationIdx].frames[i][j].y+=30;
                }
            }
            if((i==0) || (i==5)){
                for(int j=0;j<m_animations[animationIdx].frames[i].size();j++)
                {
                    m_animations[animationIdx].frames[i][j].x+=40;
                }
            }
            if((i==2) || (i==3)){
                for(int j=0;j<m_animations[animationIdx].frames[i].size();j++)
                {
                    m_animations[animationIdx].frames[i][j].x-=40;
                }
            }
        }

    }
}

void MoveCommands::PlayAnimation(Actions action, int count)
{
    int stepsTotal= m_animations[action].totalFrames;
    for(int x=0;x<count;++x){
        for(int i=0;i<stepsTotal;++i)
        {
            std::cout<<".";
            //set value
            for(int j=0;j<6;++j)
            {
                LegCoodinates lc = m_animations[action].frames[j][i];
                Hull::GetInstance()->legs_[j].SetLegCoord(lc);

            }
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }

    }
}

void MoveCommands::PlayAnimationReverse(Actions action, int count)
{
    int stepsTotal= m_animations[action].totalFrames;
    for(int x=0;x<count;++x){
        for(int i=stepsTotal-1;i>=0;--i)
        {
            std::cout<<".";
            //set value
            for(int j=0;j<6;++j)
            {
                LegCoodinates lc = m_animations[action].frames[j][i];
                Hull::GetInstance()->legs_[j].SetLegCoord(lc);

            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

}
