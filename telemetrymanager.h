#ifndef TELEMETRYMANAGER_H
#define TELEMETRYMANAGER_H
#include <map>
#include <thread>
#include <memory>
#include <zmq.hpp>
namespace Command {
    class TelemetryUpdate;
}


class TelemetryManager
{
public:
    static TelemetryManager * GetInstance();
    void setValue(std::string name, double value);
    void setValue(std::string name, bool value);
    void setValue(std::string name, std::string value);
private:
    TelemetryManager();
    std::map<std::string,double> doubleMap_;
    std::map<std::string,bool> boolMap_;
    std::map<std::string,std::string> stringMap_;

    bool boolMapUpdated_;
    bool floatMapUpdated_;
    bool stringMapUpdated_;

    zmq::context_t context;
    zmq::socket_t socket;
    void sendToSocket(Command::TelemetryUpdate& update);
};

#endif // TELEMETRYMANAGER_H
