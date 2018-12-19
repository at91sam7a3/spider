#include "visionmanager.h"
#include <thread>
#include <zmq.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv4/opencv2/highgui.hpp>

VisionManager::VisionManager()
    :cap(0)
{

}

void VisionManager::ProcessCommand(Command::CommandToCamera &toCamera)
{
    if (toCamera.command()=="ON")
    {
        StartCamera();
    }
}

/**
 *  \brief Automatic brightness and contrast optimization with optional histogram clipping
 *  \param [in]src Input image GRAY or BGR or BGRA
 *  \param [out]dst Destination image
 *  \param clipHistPercent cut wings of histogram at given percent tipical=>1, 0=>Disabled
 *  \note In case of BGRA image, we won't touch the transparency
*/



void VisionManager::StartCamera()
{
    if(!cap.isOpened())  // check if we succeeded
        return;
    std::cout<<"Done"<<std::endl;
    camera_thread_.reset(new std::thread(&VisionManager::CameraThread,this));
}

void VisionManager::CameraThread()
{
    zmq::context_t context (2);
    zmq::socket_t socket (context, ZMQ_PAIR);
    socket.bind ("tcp://*:5557");
    cv::Size size(640,480);
    cv::Mat image;
    while(true)
    {
        cap >> image;
        cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        cv::flip(image, image, -1);
        //cv::resize(image,image,size);
        //This is good place to add image preprocessing

        arucoPipeline.ProcessPipeline (image);

        cv::putText(image,
                    "Here is some text",
                    cv::Point(5,5), // Coordinates
                    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                    1.0, // Scale. 2.0 = 2x bigger
                    cv::Scalar(255,255,255), // BGR Color
                    1 // Line Thickness (Optional)
                    ); // Anti-alias (Optional)
        std::vector<uchar> buff;//buffer for coding
        std::vector<int> param(2);
        param[0] = cv::IMWRITE_JPEG_QUALITY;
        param[1] = 95;//default(95) 0-100
        cv::imencode(".jpg", image, buff, param);
        zmq::message_t frame ( buff.size());
        memcpy (frame.data (), &buff[0], buff.size());
        //And then send to socket
        socket.send(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
