#ifndef VISIONMANAGER_H
#define VISIONMANAGER_H

#include "../proto/command.pb.h"
#include <raspicam/raspicam_cv.h>
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

    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;
};

#endif // VISIONMANAGER_H
