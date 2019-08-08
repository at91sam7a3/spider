#ifndef BASEPROCEDURE_H
#define BASEPROCEDURE_H

#include <librealsense2/rs.hpp>

class BaseTask
{
public:
    BaseTask();
    virtual ~BaseTask();
    virtual void execute() = 0;
    virtual void processVideo(rs2::frameset& data)=0;
    bool IsActive();
    void MakeActive();
    void Deactivate();
private:
    bool m_active;
public:
   int m_priority;
    bool m_needVideo;
};

#endif // BASEPROCEDURE_H
