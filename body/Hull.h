#ifndef HULL_H
#define HULL_H

#include "Leg.h"
#include <vector>
//#include "Gyroscope.h"

namespace spider {

class Hull
{
public:
    //Gyroscope gyro;
    // our 6 legs inside
    std::vector <Leg> legs_;
    static Hull * GetInstance();
    void SetBodyHeight(float height);
    float GetBodyHeight();
     Hull();
private:

    static Hull * m_singlton;
};

}
#endif // HULL_H
