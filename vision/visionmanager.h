#ifndef VISIONMANAGER_H
#define VISIONMANAGER_H

#include "../proto/command.pb.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include "visionpipeline.h"
#include "ArucoPipeline.h"

class VisionManager
{
public:
    VisionManager();
    void ProcessCommand(Command::CommandToCamera&  toCamera);
    void StartCamera();

private:

    void CameraThread[[noreturn]]();
    std::shared_ptr<std::thread> camera_thread_;

   // cv::VideoCapture cap;
   // cv::Mat image;
    ArucoPipeline arucoPipeline;
    bool cameraStarted_;
    bool rgbModeCamera_;
    bool depthModeCamera_;
    bool arucoProcessing_;
};

#endif // VISIONMANAGER_H
