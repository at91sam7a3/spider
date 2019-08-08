#include "basetask.h"

BaseTask::BaseTask()
    :m_active(false)
    ,m_priority(0)
    ,m_needVideo(false)
{

}

BaseTask::~BaseTask()
{

}

bool BaseTask::IsActive()
{
    return m_active;
}

void BaseTask::MakeActive()
{
    m_active = true;
}

void BaseTask::Deactivate()
{
    m_active = false;
}
