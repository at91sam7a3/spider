#ifndef VISIONMANAGER_H
#define VISIONMANAGER_H

#include "../proto/command.pb.h"
#include <opencv2/opencv.hpp>
#include <thread>

class VisionManager
{
public:
    VisionManager();
    void ProcessCommand(Command::CommandToCamera&  toCamera);
    void StartCamera();
private:

    void CameraThread();
    std::shared_ptr<std::thread> camera_thread_;

    cv::VideoCapture cap;
    cv::Mat image;
};

#endif // VISIONMANAGER_H
