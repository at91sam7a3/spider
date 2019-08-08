#include "voicemanager.h"

VoiceManager *VoiceManager::GetInstance()
{
    static VoiceManager m_instance;
    return &m_instance;
}

void VoiceManager::Speak(std::string message)
{
    zmq::message_t reply (message.length());
    memcpy (reply.data (), message.c_str(), message.length());
    socket.send (reply);
    zmq::message_t request;

    //  Wait for next request from client
    socket.recv (&request);
}

VoiceManager::VoiceManager()
    :context(0)
    ,socket(context,ZMQ_REQ)
{
    socket.connect ("tcp://127.0.0.1:5560");
}
