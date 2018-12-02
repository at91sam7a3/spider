#ifndef IMPROVEPICTURE_H
#define IMPROVEPICTURE_H

#include "visionpipeline.h"

class ImprovePicture : public VisionPipeline
{
public:
    ImprovePicture();
    virtual ~ImprovePicture();
    void ProcessPipeline(const cv::Mat &src, cv::Mat &dst);
};

#endif // IMPROVEPICTURE_H
