#ifndef CLASSKEYPOINTSWITHNEIGHBOR_H
#define CLASSKEYPOINTSWITHNEIGHBOR_H

#include "processor/FrameProcessor.h"
#include <cv.h>

class ClassKeyPointsWithNeighbor : public FrameProcessor
{
public:
    ClassKeyPointsWithNeighbor();

    void processFrame(Video* video, Frame* frame, Mat* image, ProcessorCallback* callback);

};

#endif // CLASSKEYPOINTSWITHNEIGHBOR_H
