#include "visionmanager.h"
#include <thread>

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui.hpp>



VisionManager::VisionManager()
    :arucoPipeline()
    ,color_map(2)
    ,pipe()
    ,data()
    ,context (2)
    ,socket (context, ZMQ_PAIR)
{
   socket.bind ("tcp://*:5557");
}

VisionManager *VisionManager::GetInstance()
{
    static VisionManager m_singlton;
    return &m_singlton;
}

void VisionManager::ProcessCommand(Command::CommandToCamera &toCamera)
{
    if (toCamera.command()=="ON")
    {
        StartCamera();
    }
}

//get new frames from camera and refresh output image
void VisionManager::UpdateVideoData()
{
    data=pipe.wait_for_frames();
    rs2::frame picture = data.get_color_frame();
    const int w = picture.as<rs2::video_frame>().get_width();
    const int h = picture.as<rs2::video_frame>().get_height();
    image = cv::Mat(cv::Size(w, h), CV_8UC3, const_cast<void*>(picture.get_data()), cv::Mat::AUTO_STEP);
}

void VisionManager::SendPictureToGUI()
{
    std::vector<uchar> buff;//buffer for coding
    std::vector<int> param(2);
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = 95;//default(95) 0-100
    cv::imencode(".jpg", image, buff, param);
    zmq::message_t frame ( buff.size());
    memcpy (frame.data (), &buff[0], buff.size());
    //And then send to socket
    socket.send(frame);
}

cv::Mat *VisionManager::GetOutputImage()
{
    return &image;
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
    std::cout<<"Done"<<std::endl;
    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map(2);

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();
}
 void VisionManager::CameraThread()
{
    zmq::context_t context (2);
    zmq::socket_t socket (context, ZMQ_PAIR);
    socket.bind ("tcp://*:5557");
   /*  cv::Size size(640,480);
    //cv::Mat image;




    while(true)
    {
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
        rs2::frame depth = data.get_depth_frame().apply_filter(color_map);
        rs2::frame picture = data.get_color_frame();
        // Query frame size (width and height)
        const int w = depth.as<rs2::video_frame>().get_width();
        const int h = depth.as<rs2::video_frame>().get_height();

        // Create OpenCV matrix of size (w,h) from the colorized depth data
        cv::Mat image(cv::Size(w, h), CV_8UC3, const_cast<void*>(picture.get_data()), cv::Mat::AUTO_STEP);

       // cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        //cv::flip(image, image, -1);
        //cv::resize(image,image,size);
        //This is good place to add image preprocessing

       // arucoPipeline.ProcessPipeline (image);

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
    }*/
}
