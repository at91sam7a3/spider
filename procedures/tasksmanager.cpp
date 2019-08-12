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
    VisionManager::GetInstance();
    std::cout<<"Starting tasks loop"<<std::endl;
    while (true) {
        //refresh camera picture
        VisionManager::GetInstance()->UpdateVideoData();
  //      std::cout <<"processing video"<<std::endl;
        //process video only by task that need it
        for(auto const& it : m_taskCollection)
        {

            if( (it->IsActive() == true) && (it->m_needVideo == true)) it->processVideo( VisionManager::GetInstance()->data );
        }
  //      std::cout <<"processing reactions"<<std::endl;
        // process all tasks
        for(auto const& it : m_taskCollection)
        {
            if(it->IsActive() == true) it->execute();
        }
        //update gui with result picture
   //     std::cout <<"streaming video"<<std::endl;
        //VisionManager::GetInstance()->SendPictureToGUI();
  //      std::cout <<"sleeping"<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout <<"next iteration of AI"<<std::endl;
    }
}
