/*
 * Server.cpp
 *
 *  Created on: Jan 1, 2016
 *      Author: oleksii
 */
#include <exception>
#include "Server.h"
#include <zmq.hpp>
#include "proto/command.pb.h"
#include "servomanager.h"
#include "Communication.h"
#include "MoveCommands.h"
//#include <mosquittopp.h>

//#include "drivers/mpu6050simple.h"





namespace spider {


Server::Server() {
    // TODO Auto-generated constructor stub

}

Server::~Server() {
    // TODO Auto-generated destructor stub
}



void Server::startServer() {

    //    mosquitto_thread_.reset(new std::thread(&Server::mosquittoThread,this));
    server_thread_.reset(new std::thread(&Server::zeromqTrhread,this));
    MoveCommands::GetInstance()->Halt();
}



void Server::zeromqTrhread()
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
        case COMMAND_TO_SERVO:
        {
            std::cout << "COMMAND_TO_SERVO" << std::endl;

            Command::CommandToServo  toServo;
            toServo.ParseFromArray(static_cast<char*>(request.data())+1,static_cast<int>(request.size()-1));
            std::cout << toServo.name().c_str()<<std::endl;
            Command::ResponceFromServo fromServo = ServoManager::processServoCommand(toServo);
            std::string replyString(fromServo.SerializeAsString());
            //  Send reply back to client
            zmq::message_t reply (replyString.length());
            memcpy (reply.data (), replyString.c_str(), replyString.length());
            socket.send (reply);
        }
            break;
        case MOVE_COMMAND:
        {
            Command::MoveCommand  mc;
            mc.ParseFromArray(static_cast<char*>(request.data())+1,static_cast<int>(request.size()-1));
            if(mc.has_steps())
                MoveCommands::GetInstance()->DoAction(mc.command(), mc.steps());

            if(mc.has_parameter())
                MoveCommands::GetInstance()->DoAction2(mc.command(), mc.parameter());

            zmq::message_t reply (1);
            char rep=EMPTY_ANSWER;
            memcpy (reply.data (), &rep, 1);
            socket.send (reply);
        }
            break;
        case LEG_MOVEMENT:
        {
            Command::LegMoveCommand lmc;
            lmc.ParseFromArray(static_cast<char*>(request.data())+1,static_cast<int>(request.size()-1));
            LegCoodinates lc;
            lc.height=lmc.z();
            lc.x = lmc.x();
            lc.y = lmc.y();
            Hull::GetInstance()->legs_[lmc.leg()].SetLegCoord(lc);
//reply empty answer
            zmq::message_t reply (1);
            char rep=EMPTY_ANSWER;
            memcpy (reply.data (), &rep, 1);
            socket.send (reply);
        }
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


