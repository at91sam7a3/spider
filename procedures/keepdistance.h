#ifndef KEEPDISTANCE_H
#define KEEPDISTANCE_H

#include "baseprocedure.h"

//This procedure try to keep distance to obstacle

class KeepDistance : public BaseProcedure
{
public:
    KeepDistance();
    virtual void execute();
};

#endif // KEEPDISTANCE_H
