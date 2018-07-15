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



namespace spider {

class Server
{
public:
	Server();
	~Server();
public:
	void startServer();
    static std::string commandsProtocol1(std::string&);
private:

    void zeromqTrhread();
//    void mosquittoThread();
    std::shared_ptr<std::thread> server_thread_;
//    std::shared_ptr<std::thread> mosquitto_thread_;

};

} /* namespace spider */

#endif /* SERVER_H_ */
