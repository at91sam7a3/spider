/*
 * Server.cpp
 *
 *  Created on: Jan 1, 2016
 *      Author: oleksii
 */
#include <exception>
#include "Server.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>


#include <mosquittopp.h>

//#include "drivers/mpu6050simple.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
namespace spider {


Server::Server() {
    // TODO Auto-generated constructor stub

}

Server::~Server() {
    // TODO Auto-generated destructor stub
}



void Server::startServer() {

    mosquitto_thread_.reset(new std::thread(&Server::mosquittoThread,this));
}

std::string Server::commandsProtocol1(std::string &line )
{
      std::stringstream ss;

    std::cout<<"WRITING "<<ss.str();
    return (ss.str());
}



class MqttWrapper;
static MqttWrapper * mqttHdl = nullptr;

class MqttWrapper : public mosqpp::mosquittopp
{

public:
    MqttWrapper(const char *id) : mosquittopp(id, false)
    {
        mosqpp::lib_init();                                          // Initialize libmosquitto
        int keepalive = 120; // seconds
        username_pw_set("spider", "tiny2313");
        fprintf(stderr,"Connecting ...%d\n",connect("m11.cloudmqtt.com", 11727, keepalive));
    }

    virtual ~MqttWrapper()
    {
        disconnect();
        mosqpp::lib_cleanup();
    }

    void on_connect(int rc)
    {
        fprintf(stdout,"Connected with code %d. \n", rc);
        if (rc == 0)
        {
        }
    }

    void on_message(const struct mosquitto_message *message)
    {
        fprintf(stderr, "on_message: '%s'\n", (char*)(message->payload));
        try
        {
            std::string line((char*)(message->payload));

             std::string res = Server::commandsProtocol1(line);
             if(mqttHdl)
                mqttHdl->publish(NULL, "spider2app", res.length(), res.c_str(),1);
        }
        catch(std::exception& e)
        {
            std::cerr<<e.what()<<std::endl;

        }

    }

    void on_subcribe(int mid, int qos_count, const int *granted_qos)
    {
        fprintf(stderr, "Subscription succeeded.  \n");
    }

    virtual void on_disconnect(int rc)
    {
    }

    virtual void on_publish(int mid)
    {
        //fprintf(stderr, "on publish\n");
    }

    virtual void on_unsubscribe(int mid)
    {
        // fprintf(stderr, "on_unsubscribe\n");
    }

    virtual void on_log(int level, const char *str)
    {
        //fprintf(stderr, "log %s\n", str);
    }

    virtual void on_error()
    {
        fprintf(stderr, "error\n");
    }

};

void Server::mosquittoThread()
{
    std::cout<<"mqtt thread started"<<std::endl;
    MqttWrapper * mqttHdl;
    mqttHdl = new MqttWrapper("myDevice");
    const char* tstr= "my_message";
    int res;
    mqttHdl->subscribe(NULL,"app2spider", 1);

    while(1)
    {
        res = mqttHdl->loop();
        if (res){
            fprintf(stderr, "reconnect\n");
            mqttHdl->reconnect();
        }
        usleep(100);

    }
    std::cout<<"mqtt thread ended"<<std::endl;
}


} /* namespace spider */


