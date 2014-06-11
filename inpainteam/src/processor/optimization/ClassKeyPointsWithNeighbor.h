#ifndef CLASSKEYPOINTSWITHNEIGHBOR_H
#define CLASSKEYPOINTSWITHNEIGHBOR_H

#include "processor/FrameProcessor.h"
#include <cv.h>

class ProcessorCallback;

class ClassKeyPointsWithNeighbor : public FrameProcessor
{
public:
    ClassKeyPointsWithNeighbor();

    void processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback);

};

#endif // CLASSKEYPOINTSWITHNEIGHBOR_H
