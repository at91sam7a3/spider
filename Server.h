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

#include "vision/visionmanager.h"

namespace spider {

class Server
{
public:
	Server();
	~Server();
public:
    void startServer();

private:
    void settingThread();
    void ordersThread();
//    void mosquittoThread();
    std::shared_ptr<std::thread> orders_thread_;
    std::shared_ptr<std::thread> settings_thread_;
//    std::shared_ptr<std::thread> mosquitto_thread_;

   VisionManager visionManager_;

};

} /* namespace spider */

#endif /* SERVER_H_ */
