#include "telemetrymanager.h"
#include "proto/command.pb.h"

TelemetryManager * TelemetryManager::GetInstance()
{
    static TelemetryManager m_instance ;
    return &m_instance;
}

void TelemetryManager::setValue(std::string name, double value)
{
 /*   if(doubleMap_.find(name) != doubleMap_.end())
    {
        if(doubleMap_[name]==value)
        {
            return;
        }
    }*/

    doubleMap_[name] = value;
    Command::TelemetryUpdate update;
    update.set_name(name);
    update.set_fvalue(value);
    sendToSocket(update);
}

void TelemetryManager::setValue(std::string name, bool value)
{
    if(boolMap_.find(name) != boolMap_.end())
    {
        if(boolMap_[name]==value)
        {
            return;
        }
    }

    boolMap_[name] = value;
    Command::TelemetryUpdate update;
    update.set_name(name);
    update.set_bvalue(value);
    sendToSocket(update);
}

void TelemetryManager::sendToSocket(Command::TelemetryUpdate& update)
{
    std::string tmp(update.SerializeAsString());
    zmq::message_t message(tmp.size());
    memcpy (message.data (), tmp.c_str(), tmp.length());
    socket.send(message);
}

void TelemetryManager::setValue(std::string name, std::string value)
{
    if(stringMap_.find(name) != stringMap_.end())
    {
        if(stringMap_[name]==value)
        {
            return;
        }
    }

    stringMap_[name]=value;
    Command::TelemetryUpdate update;
    update.set_name(name);
    update.set_svalue(value);
    sendToSocket(update);
}

TelemetryManager::TelemetryManager()
    :boolMapUpdated_(false)
    ,floatMapUpdated_(false)
    ,stringMapUpdated_(false)
    ,context(1)
    ,socket(context,ZMQ_PAIR)
{
    socket.bind ("tcp://*:5558");
}
