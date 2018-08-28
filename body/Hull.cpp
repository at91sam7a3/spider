#include "Hull.h"

namespace spider {

 Hull * Hull::m_singlton = nullptr;

Hull::Hull()
{
    for (int i=0; i<6;++i)
    {
        Leg leg;
        leg.SetLegIndex(i);
        legs_.push_back(leg);
    }
}

Hull *Hull::GetInstance()
{
    if(!m_singlton)
        m_singlton = new Hull();
    return m_singlton;
}

void Hull::SetBodyHeight(float height)
{
    for (size_t i=0; i<6;++i)
    {
        legs_[i].bodyHeight_=height;
        legs_[i].RecalcAngles();
    }
}

float Hull::GetBodyHeight()
{
    return legs_[0].bodyHeight_;
}



}
