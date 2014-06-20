#ifndef CANNYFLOWTRACE_H
#define CANNYFLOWTRACE_H

#include <cv.h>

class Video;
class Frame;

class CannyFlowTrace
{
public:
    CannyFlowTrace();
    virtual ~ CannyFlowTrace();

    static cv::Mat getCannyPoints(Frame* frame);
    virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);
};

#endif // CANNYFLOWTRACE_H
