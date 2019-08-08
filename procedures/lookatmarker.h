#ifndef LOOKATMARKER_H
#define LOOKATMARKER_H

#include "basetask.h"

class LookAtMarker : public BaseTask
{
public:
    LookAtMarker();
    virtual void execute();
    virtual void processVideo(rs2::frameset& data);
};

#endif // LOOKATMARKER_H
