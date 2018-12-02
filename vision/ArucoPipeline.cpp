#include "ArucoPipeline.h"
#include <opencv2/highgui.hpp>


ArucoPipeline::ArucoPipeline()
{
    // camera parameters are read from somewhere
    // cv::aruco::readCameraParameters(cameraMatrix, distCoeffs);
    detectorParams = cv::aruco::DetectorParameters::create();

    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);
    std::cout << "Reading config for camera ";
    bool readOk = readDetectorParameters("detector_params.yml" , detectorParams);
    if(!readOk) {
        std::cerr << "Invalid detector parameters file" << std::endl;
    }
    else
        std::cout<<"Ok"<<std::endl;
}

ArucoPipeline::~ArucoPipeline()
{

}

void ArucoPipeline::ProcessPipeline(const cv::Mat &src)
{


    std::vector< int > ids;
    std::vector< std::vector< cv::Point2f > > corners, rejected;
    std::vector< cv::Vec3d > rvecs, tvecs;

    // detect markers and estimate pose
    cv::aruco::detectMarkers(src, dictionary, corners, ids, detectorParams, rejected);
    // if at least one marker detected
    if (ids.size() > 0)
    {
        std::cout<<"F";
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



/**
 */
bool ArucoPipeline::readDetectorParameters(std::string filename, cv::Ptr<cv::aruco::DetectorParameters> &params) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["adaptiveThreshWinSizeMin"] >> params->adaptiveThreshWinSizeMin;
    fs["adaptiveThreshWinSizeMax"] >> params->adaptiveThreshWinSizeMax;
    fs["adaptiveThreshWinSizeStep"] >> params->adaptiveThreshWinSizeStep;
    fs["adaptiveThreshConstant"] >> params->adaptiveThreshConstant;
    fs["minMarkerPerimeterRate"] >> params->minMarkerPerimeterRate;
    fs["maxMarkerPerimeterRate"] >> params->maxMarkerPerimeterRate;
    fs["polygonalApproxAccuracyRate"] >> params->polygonalApproxAccuracyRate;
    fs["minCornerDistanceRate"] >> params->minCornerDistanceRate;
    fs["minDistanceToBorder"] >> params->minDistanceToBorder;
    fs["minMarkerDistanceRate"] >> params->minMarkerDistanceRate;
    fs["cornerRefinementMethod"] >> params->cornerRefinementMethod;
    fs["cornerRefinementWinSize"] >> params->cornerRefinementWinSize;
    fs["cornerRefinementMaxIterations"] >> params->cornerRefinementMaxIterations;
    fs["cornerRefinementMinAccuracy"] >> params->cornerRefinementMinAccuracy;
    fs["markerBorderBits"] >> params->markerBorderBits;
    fs["perspectiveRemovePixelPerCell"] >> params->perspectiveRemovePixelPerCell;
    fs["perspectiveRemoveIgnoredMarginPerCell"] >> params->perspectiveRemoveIgnoredMarginPerCell;
    fs["maxErroneousBitsInBorderRate"] >> params->maxErroneousBitsInBorderRate;
    fs["minOtsuStdDev"] >> params->minOtsuStdDev;
    fs["errorCorrectionRate"] >> params->errorCorrectionRate;
    return true;
}
