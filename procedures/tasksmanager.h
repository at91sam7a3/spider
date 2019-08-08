#ifndef PROCEDURESMANAGER_H
#define PROCEDURESMANAGER_H

#include <list>
#include <thread>

#include "basetask.h"

class TasksManager
{
public:
    TasksManager();

    void StartTasksLoop();
private:
    std::list<std::unique_ptr<BaseTask>> m_taskCollection;
    std::unique_ptr<std::thread> m_tasks_thread;
    void TasksLoop[[noreturn]]();
};

#endif // PROCEDURESMANAGER_H
