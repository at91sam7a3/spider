/*
 * Server.cpp
 *
 *  Created on: Jan 1, 2016
 *      Author: oleksii
 */

#include "Server.h"

#include "proto/command.pb.h"
#include "servomanager.h"
#include "Communication.h"
//#include <mosquittopp.h>

//#include "drivers/mpu6050simple.h"
#include <zmq.hpp>
#include <exception>



namespace spider {


Server::Server() {
    // TODO Auto-generated constructor stub

}

Server::~Server() {
    // TODO Auto-generated destructor stub
}



void Server::startServer(bool travelMode) {

    //    mosquitto_thread_.reset(new std::thread(&Server::mosquittoThread,this));
    if(travelMode){
        //MoveCommands::GetInstance()->Sleep();
        std::cout<<"Going to turn off"<<std::endl;
        platform.Sleep();
        return;
    }
    orders_thread_.reset(new std::thread(&Server::ordersThread,this));
    settings_thread_.reset(new std::thread(&Server::settingThread,this));

}


void Server::settingThread()
{
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_PAIR);
    socket.bind ("tcp://*:5556");
    while (true) {
        zmq::message_t rcv_message;

        //  Wait for next request from client
        socket.recv (&rcv_message);
        std::cout << "Received settings command" << std::endl;
        char commandType = *(static_cast<char*>(rcv_message.data()));
        switch(commandType)
        {
        case COMMAND_TO_CAMERA:
        {

           /* Command::CommandToCamera  toCamera;
            toCamera.ParseFromArray(static_cast<char*>(rcv_message.data())+1,static_cast<int>(rcv_message.size()-1));
            std::cout << toCamera.command().c_str()<<std::endl;
            VisionManager::GetInstance()->ProcessCommand(toCamera);*/
            taskManager.StartTasksLoop();

        }
            break;
        }

    }
}

void Server::ordersThread()
{
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");
    while (true) {
        zmq::message_t request;

        //  Wait for next request from client
        socket.recv (&request);
        std::cout << "Received command" << std::endl;

        char commandType = *(static_cast<char*>(request.data()));
        switch(commandType)
        {
        case MOVE_COMMAND:
        {
            std::cout << "Received move command" << std::endl;
            Command::MoveCommand  mc;
            mc.ParseFromArray(static_cast<char*>(request.data())+1,static_cast<int>(request.size()-1));
            if(mc.has_x())
            {
                std::cout << "Received command to move xy" << std::endl;
                platform.GoToCoordinates(vec2f(mc.x(),mc.y()));
            }
            if(mc.has_rotation_before()){
                std::cout << "Received command to turn a" << std::endl;
                platform.Turn(mc.rotation_before());
            }
            zmq::message_t reply (1);
            char rep=EMPTY_ANSWER;
            memcpy (reply.data (), &rep, 1);
            socket.send (reply);
        }
            break;

        default:

            break;
        }
    }
}


/*
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

*/
} /* namespace spider */


