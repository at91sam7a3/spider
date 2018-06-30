/*
 * Server.h
 *
 *  Created on: Jan 1, 2016
 *      Author: oleksii
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <thread>
#include <memory>
#include <boost/asio.hpp>



namespace spider {

class Server : private boost::noncopyable
{
public:
	Server();
	~Server();
public:
	void startServer();
    static std::string commandsProtocol1(std::string&);
private:

	void serverTrhread();
    void mosquittoThread();
    std::shared_ptr<std::thread> server_thread_;
    std::shared_ptr<std::thread> mosquitto_thread_;

};

} /* namespace spider */

#endif /* SERVER_H_ */
