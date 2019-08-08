#ifndef VISIONMANAGER_H
#define VISIONMANAGER_H

#include "visionpipeline.h"
#include "ArucoPipeline.h"
#include "../proto/command.pb.h"

#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include <zmq.hpp>
#include <thread>

class VisionManager
{
public:
    static VisionManager *  GetInstance();
    void ProcessCommand(Command::CommandToCamera&  toCamera);
    void UpdateVideoData();
    void SendPictureToGUI();
    cv::Mat* GetOutputImage();
    rs2::frameset data;
private:
    VisionManager();
    void StartCamera();
    void CameraThread[[noreturn]]();
    std::shared_ptr<std::thread> camera_thread_;

    // cv::VideoCapture cap;
    // cv::Mat image;
    ArucoPipeline arucoPipeline;
    rs2::colorizer color_map;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;

    cv::Mat image;

    zmq::context_t context;
    zmq::socket_t socket;
};

#endif // VISIONMANAGER_H
