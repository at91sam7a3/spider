#include "Head.h"


namespace spider {

Head* Head::m_singlton = 0;



Head::Head()
{

}

Head *Head::GetInstance()
{
    if(!m_singlton)
        m_singlton = new Head();
    return m_singlton;
}

void Head::SetVerticalAngle(float a)
{
    if(a>30) return;
    if(a<-30) return;
   // CalibratedServoCollection::GetInstance()->servos_[18]->SetAngle(a);
}

void Head::SetHorizontalAngle(float a)
{
    if(a>30) return;
    if(a<-30) return;
    //CalibratedServoCollection::GetInstance()->servos_[19]->SetAngle(a);
}


}
