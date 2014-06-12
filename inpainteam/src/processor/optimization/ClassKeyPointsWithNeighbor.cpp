#include "ClassKeyPointsWithNeighbor.h"
#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/ExtendedPoint.h"
#include "entities/PointTrace.h"
#include "processor/optimization/MovementReprojection.h"

#include <iostream>

using namespace std;
using namespace cv;

ClassKeyPointsWithNeighbor::ClassKeyPointsWithNeighbor()
{
}


void ClassKeyPointsWithNeighbor::processFrame(Video* video, Frame* frame, Mat* image, ProcessorCallback* callback){
    MovementReprojection movementReprojection;
    movementReprojection.processDoubleFrame(video, frame, frame);
}
