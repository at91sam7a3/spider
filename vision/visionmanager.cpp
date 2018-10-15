#include "visionmanager.h"
#include <thread>
#include <zmq.hpp>

VisionManager::VisionManager()
{

}

void VisionManager::ProcessCommand(Command::CommandToCamera &toCamera)
{
    if (toCamera.command()=="ON")
    {
        StartCamera();
    }
}

void VisionManager::StartCamera()
{
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
    Camera.set( CV_CAP_PROP_FRAME_WIDTH, 1280);
    Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 800);
    //Open camera
    std::cout<<"Opening Camera..."<<std::endl;
    if (!Camera.open())
    {
        std::cerr<<"Error opening the camera"<<std::endl;
        return;
    }
    camera_thread_.reset(new std::thread(&VisionManager::CameraThread,this));
}

void VisionManager::CameraThread()
{
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_PUB);
    socket.bind ("tcp://*:5557");
    while(true)
    {
    Camera.grab();
    Camera.retrieve ( image);
    //This is good place to add image preprocessing
    std::vector<uchar> buff;//buffer for coding
    std::vector<int> param(2);
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = 80;//default(95) 0-100
    cv::imencode(".jpg", image, buff, param);
    zmq::message_t frame (sizeof(unsigned int) + buff.size());
    //And then send to socket
    }
}
