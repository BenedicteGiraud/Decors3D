#ifndef CANNYFLOWTRACE_H
#define CANNYFLOWTRACE_H

#include <cv.h>

#include "processor/DoubleFrameProcessor.h"
#include "entities/Video.h"

class Frame;

class CannyFlowTrace : public DoubleFrameProcessor
{
public:
    Video cannyVideo;

    CannyFlowTrace();
    virtual ~ CannyFlowTrace();

    Video * getCannyVideo();
    static cv::Mat getCannyPoints(Frame* frame);
    virtual void processStart(Video* video, Frame* firstFrame);
    virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);
};

#endif // CANNYFLOWTRACE_H
