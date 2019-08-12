#ifndef KEEPDISTANCE_H
#define KEEPDISTANCE_H

#include "basetask.h"

//This procedure try to keep distance to obstacle

class KeepDistance : public BaseTask
{
public:
    KeepDistance();
    virtual void execute();
    virtual void processVideo(rs2::frameset& data);

private:
    double m_distanceFromObstacle;
};

#endif // KEEPDISTANCE_H
