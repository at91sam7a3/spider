#include "tasksmanager.h"
#include "keepdistance.h"
#include "lookatmarker.h"
#include "../vision/visionmanager.h"

TasksManager::TasksManager()
{
    m_taskCollection.emplace_back(std::unique_ptr<BaseTask>(new KeepDistance()));
    m_taskCollection.emplace_back(std::unique_ptr<BaseTask>(new LookAtMarker()));
}

void TasksManager::StartTasksLoop()
{
    m_tasks_thread.reset(new std::thread(&TasksManager::TasksLoop,this));
}

void TasksManager::TasksLoop()
{
    while (true) {
        //refresh camera picture
        VisionManager::GetInstance()->UpdateVideoData();

        //process video only by task that need it
        for(auto const& it : m_taskCollection)
        {
            if( (it->IsActive() == true) && (it->m_needVideo == true)) it->processVideo( VisionManager::GetInstance()->data );
        }

        // process all tasks
        for(auto const& it : m_taskCollection)
        {
            if(it->IsActive() == true) it->execute();
        }
        //update gui with result picture
        VisionManager::GetInstance()->SendPictureToGUI();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
