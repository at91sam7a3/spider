#include "visionmanager.h"
#include <thread>
#include <zmq.hpp>
#include "opencv2/imgproc/imgproc.hpp"

VisionManager::VisionManager()
    :cap(0)
{

}

void VisionManager::ProcessCommand(Command::CommandToCamera &toCamera)
{
    if (toCamera.command()=="ON")
    {
        StartCamera();
    }
}

/**
 *  \brief Automatic brightness and contrast optimization with optional histogram clipping
 *  \param [in]src Input image GRAY or BGR or BGRA
 *  \param [out]dst Destination image
 *  \param clipHistPercent cut wings of histogram at given percent tipical=>1, 0=>Disabled
 *  \note In case of BGRA image, we won't touch the transparency
*/
void BrightnessAndContrastAuto(const cv::Mat &src, cv::Mat &dst, float clipHistPercent=0)
{

    CV_Assert(clipHistPercent >= 0);
    CV_Assert((src.type() == CV_8UC1) || (src.type() == CV_8UC3) || (src.type() == CV_8UC4));

    int histSize = 256;
    float alpha, beta;
    double minGray = 0, maxGray = 0;

    //to calculate grayscale histogram
    cv::Mat gray;
    if (src.type() == CV_8UC1) gray = src;
    else if (src.type() == CV_8UC3) cvtColor(src, gray, CV_BGR2GRAY);
    else if (src.type() == CV_8UC4) cvtColor(src, gray, CV_BGRA2GRAY);
    if (clipHistPercent == 0)
    {
        // keep full available range
        cv::minMaxLoc(gray, &minGray, &maxGray);
    }
    else
    {
        cv::Mat hist; //the grayscale histogram

        float range[] = { 0, 256 };
        const float* histRange = { range };
        bool uniform = true;
        bool accumulate = false;
        calcHist(&gray, 1, 0, cv::Mat (), hist, 1, &histSize, &histRange, uniform, accumulate);

        // calculate cumulative distribution from the histogram
        std::vector<float> accumulator(histSize);
        accumulator[0] = hist.at<float>(0);
        for (int i = 1; i < histSize; i++)
        {
            accumulator[i] = accumulator[i - 1] + hist.at<float>(i);
        }

        // locate points that cuts at required value
        float max = accumulator.back();
        clipHistPercent *= (max / 100.0); //make percent as absolute
        clipHistPercent /= 2.0; // left and right wings
        // locate left cut
        minGray = 0;
        while (accumulator[minGray] < clipHistPercent)
            minGray++;

        // locate right cut
        maxGray = histSize - 1;
        while (accumulator[maxGray] >= (max - clipHistPercent))
            maxGray--;
    }

    // current range
    float inputRange = maxGray - minGray;

    alpha = (histSize - 1) / inputRange;   // alpha expands current range to histsize range
    beta = -minGray * alpha;             // beta shifts current range so that minGray will go to 0

    // Apply brightness and contrast normalization
    // convertTo operates with saurate_cast
    src.convertTo(dst, -1, alpha, beta);

    // restore alpha channel from source
    if (dst.type() == CV_8UC4)
    {
        int from_to[] = { 3, 3};
        cv::mixChannels(&src, 4, &dst,1, from_to, 1);
    }
    return;
}


void VisionManager::StartCamera()
{
    if(!cap.isOpened())  // check if we succeeded
        return;
    std::cout<<"Done"<<std::endl;
    camera_thread_.reset(new std::thread(&VisionManager::CameraThread,this));
}

void VisionManager::CameraThread()
{
    zmq::context_t context (2);
    zmq::socket_t socket (context, ZMQ_PAIR);
    socket.bind ("tcp://*:5557");
    cv::Size size(640,480);
    while(true)
    {
       cap >> image;
        cv::flip(image, image, -1);
        cv::resize(image,image,size);
        //This is good place to add image preprocessing
        std::vector<uchar> buff;//buffer for coding
        std::vector<int> param(2);
        param[0] = cv::IMWRITE_JPEG_QUALITY;
        param[1] = 95;//default(95) 0-100
        cv::imencode(".jpg", image, buff, param);
        zmq::message_t frame ( buff.size());
        memcpy (frame.data (), &buff[0], buff.size());
        //And then send to socket
        socket.send(frame);
         std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
