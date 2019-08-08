#include "keepdistance.h"
#include <iostream>
#define DISTANCE_TO_KEEP 400 //mm

KeepDistance::KeepDistance()
{
    m_priority=0;
    m_needVideo=true;
}

void KeepDistance::execute()
{
    //get distance
    //if it more than needed then go forward, else  go back
    if ((m_distanceFromObstacle >0) && m_distanceFromObstacle < DISTANCE_TO_KEEP)
    {
        float distance_to_walk_back = DISTANCE_TO_KEEP - m_distanceFromObstacle;

    }
}

void KeepDistance::processVideo(rs2::frameset &data)
{
    rs2::depth_frame depth = data.get_depth_frame();
    m_distanceFromObstacle = depth.get_distance(depth.get_width()/2, depth.get_height()/2);
    std::cout <<"Got distance from object "<< m_distanceFromObstacle<<std::endl;
}

