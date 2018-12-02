#ifndef VISIONPIPELINE_H
#define VISIONPIPELINE_H

#include <opencv2/aruco.hpp>

class VisionPipeline
{
public:
    VisionPipeline(){;}
    virtual ~VisionPipeline(){;};
    virtual void ProcessPipeline(const cv::Mat &src)=0;

};

#endif // VISIONPIPELINE_H
