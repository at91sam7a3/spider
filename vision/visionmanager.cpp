#include "visionmanager.h"
#include <thread>
#include <zmq.hpp>
#include "opencv2/imgproc/imgproc.hpp"

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
    Camera.set( CV_CAP_PROP_FRAME_WIDTH, 2592);
    Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 1944);

   // Camera.set(CV_CAP_PROP_EXPOSURE, 100); //-1 is auto, values range from 0 to 100
  //  Camera.set(CV_CAP_PROP_WHITE_BALANCE_RED_V, 100); //values range from 0 to 100, -1 auto whitebalance
   // Camera.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U, 100); //values range from 0 to 100,  -1 auto whitebalance

    //Open camera
    std::cout<<"Opening Camera..."<<std::endl;
    if (!Camera.open())
    {
        std::cerr<<"Error opening the camera"<<std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //Camera.set(CV_CAP_PROP_EXPOSURE, -1); //-1 is auto, values range from 0 to 100
    Camera.set(CV_CAP_PROP_BRIGHTNESS,50);
   // Camera.set(CV_CAP_PROP_CONTRAST,0);
   // Camera.set(CV_CAP_PROP_GAIN, 100); // values range from 0 to 100
  //  Camera.set(CV_CAP_PROP_WHITE_BALANCE_RED_V, -1); //values range from 0 to 100, -1 auto whitebalance
   // Camera.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U, -1); //values range from 0 to 100,  -1 auto whitebalance
    std::cout<<"Done"<<std::endl;
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    camera_thread_.reset(new std::thread(&VisionManager::CameraThread,this));
}

void VisionManager::CameraThread()
{
    zmq::context_t context (2);
    zmq::socket_t socket (context, ZMQ_PAIR);
    socket.bind ("tcp://*:5557");
    cv::Size size(1280,800);
    while(true)
    {
        Camera.grab();
        Camera.retrieve ( image);
        cv::flip(image, image, -1);
        cv::resize(image,image,size);

        //This is good place to add image preprocessing
        std::vector<uchar> buff;//buffer for coding
        std::vector<int> param(2);
        param[0] = cv::IMWRITE_JPEG_QUALITY;
        param[1] = 95;//default(95) 0-100
        cv::imencode(".jpg", image, buff, param);
        zmq::message_t frame ( buff.size());
        memcpy (frame.data (), &buff[0], buff.size());
        //And then send to socket
        socket.send(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
