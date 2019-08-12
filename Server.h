/*
 * Server.h
 *
 *  Created on: Jan 1, 2016
 *      Author: oleksii
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <memory>
#include "platform.h"
#include "vision/visionmanager.h"
#include "procedures/tasksmanager.h"

namespace spider {

class Server
{
public:
	Server();
	~Server();
public:
    void startServer(bool travelMode);

private:
    void settingThread();
    void ordersThread();
//    void mosquittoThread();
    std::unique_ptr<std::thread> orders_thread_;
    std::unique_ptr<std::thread> settings_thread_;
//    std::shared_ptr<std::thread> mosquitto_thread_;

   TasksManager taskManager;
};

} /* namespace spider */

#endif /* SERVER_H_ */
