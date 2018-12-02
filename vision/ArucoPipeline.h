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
    virtual void ProcessPipeline(const cv::Mat &src) override;
private:
    cv::Ptr<cv::aruco::Dictionary> dictionary ;
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams;
    cv::Mat cameraMatrix, distCoeffs;

    bool readCameraParameters(std::string filename, cv::Mat &camMatrix, cv::Mat &distCoeffs);
    bool readDetectorParameters(std::string filename, cv::Ptr<cv::aruco::DetectorParameters> &params);
};

#endif // ARUCOPIPELINE_H
