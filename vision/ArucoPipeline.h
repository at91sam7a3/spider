#ifndef ARUCOPIPELINE_H
#define ARUCOPIPELINE_H

#include "visionpipeline.h"
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

class ArucoPipeline : public VisionPipeline
{
public:
    ArucoPipeline();
    ~ArucoPipeline();
    virtual void ProcessPipeline(cv::Mat &src) override;
private:
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams;
    cv::Ptr<cv::aruco::Dictionary> dictionary ;
    cv::Mat cameraMatrix, distCoeffs;

    bool readCameraParameters(std::string filename, cv::Mat &camMatrix, cv::Mat &distCoeffs);
};

#endif // ARUCOPIPELINE_H
