#include "ArucoPipeline.h"
#include <opencv2/highgui.hpp>


ArucoPipeline::ArucoPipeline()
    : detectorParams(cv::aruco::DetectorParameters::create())
    , dictionary (cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(16)))
{

}

ArucoPipeline::~ArucoPipeline()
{

}

void ArucoPipeline::ProcessPipeline(cv::Mat &src)
{
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f> > corners;
    std::vector<cv::Vec3d> rvecs, tvecs;
    cv::aruco::detectMarkers(src, dictionary, corners, ids, detectorParams);
    if (ids.size() > 0)
    {
        cv::aruco::drawDetectedMarkers(src, corners, ids);
    }
}

bool ArucoPipeline::readCameraParameters(std::string filename, cv::Mat &camMatrix, cv::Mat &distCoeffs) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["camera_matrix"] >> camMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    return true;
}


