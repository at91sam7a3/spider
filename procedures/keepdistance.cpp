#include "keepdistance.h"
#include "../platform.h"
#include <iostream>
#define DISTANCE_TO_KEEP 400 //mm

KeepDistance::KeepDistance()
{
    m_priority=0;
    m_needVideo=true;
}

void KeepDistance::execute()
{
    std::cout << "executing movement for distance task"<<std::endl;;
    if((spider::Platform::GetInstance()->state_ != spider::MovementState::Idle) && (spider::Platform::GetInstance()->state_ != spider::MovementState::Sleeping))
        return;

    //get distance
    //if it more than needed then go forward, else  go back
    if ((m_distanceFromObstacle >0) && m_distanceFromObstacle < DISTANCE_TO_KEEP)
    {


        float distance_to_walk_back = DISTANCE_TO_KEEP - m_distanceFromObstacle;
        std::cout <<" Perfoming reverse movement for "<<distance_to_walk_back<<std::endl;
        spider::Platform::GetInstance()->MoveForward(-distance_to_walk_back);

    }
}

void KeepDistance::processVideo(rs2::frameset &data)
{
    std::cout<<"Processing video input" <<std::endl;
    rs2::depth_frame depth = data.get_depth_frame();
    m_distanceFromObstacle = depth.get_distance(depth.get_width()/2, depth.get_height()/2) * 1000;
    std::cout <<"Got distance from object "<< m_distanceFromObstacle<<std::endl;
}

