#ifndef VOICEMANAGER_H
#define VOICEMANAGER_H

#include <string>
#include <zmq.hpp>

class VoiceManager
{
public:
    static VoiceManager * GetInstance();
    void Speak(std::string message);
private:
    VoiceManager();
    zmq::context_t context;
    zmq::socket_t socket;
};

#endif // VOICEMANAGER_H
