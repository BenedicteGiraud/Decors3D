#include "CannyFlowTrace.h"

#include <cv.h>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/PointTrace.h"
#include "entities/ExtendedPoint.h"

#include "processor/detection/KeyPointProcessor.h"
#include "processor/detection/KeyPointTraceProcessor.h"

using namespace std;

/** Purpose : create the trace
 * Define the point for the next frame the nearest from the one at the current frame */

CannyFlowTrace::CannyFlowTrace()
{
}

CannyFlowTrace::~CannyFlowTrace(){
}

Mat CannyFlowTrace::getCannyPoints(Frame *frame){
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.08;
    Mat channels[3];
    split(frame->image, channels);
    Mat dest;
    for(int i=0; i<3; i++) {
        Mat tmp;
        //cornerHarris( channels[i], tmp, blockSize, apertureSize, k, BORDER_DEFAULT );
        Canny(channels[i], tmp, 150, 200, 3);
        if(dest.rows == 0) {
            dest = tmp;
        }
        else {
            dest += tmp / 3;
        }
    }
    return dest;
}

void CannyFlowTrace::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2){
    Mat cannyMat1;
    Mat cannyMat2;

    cannyMat1 = getCannyPoints(frame1);
    // boucle for with a thresh : Thresh in order to keep only points which have a value bigger than 200
    for (int row = 0; row < frame1->image.rows; row++){
        for (int col = 0; col < frame1->image.cols; col++) {
            // it is not a canny point : we do not search a keypoint in thoses values
            if (cannyMat1.at<uchar>(row, col) < 250) {
                continue;
            }
            // this is a canny point : we look for the keypoint in the canypoints
            Point2f keypoint = Point2f(row, col);
            Mat descr1;
            KeyPointProcessor::extractPatchDescriptor(frame1->image, descr1, keypoint);

            double distMin = DBL_MAX;
            Point2f bestPoint ;
            // Compare this keypoint to thoses of the next frame
            for (int row2 = 0; row2 < frame2->image.rows; row2++){
                for (int col2 = 0; col2 < frame2->image.cols; col2++) {
                    // it is not a canny point : we do not search a keypoint in thoses values
                    if (cannyMat2.at<uchar>(row2, col2) < 250) {
                        continue;
                    }
                    // this is a canny point : we look for the keypoint in the canypoints
                    Point2f keypoint2 = Point2f(row2, col2);
                    Mat descr2;
                    KeyPointProcessor::extractPatchDescriptor(frame2->image, descr2, keypoint2);
                    double dist = KeyPointProcessor::descriptorDistance(descr1, descr2);
                    if (dist < distMin) {
                        cout<< "I was here ;-) " << endl;
                        distMin = dist;
                        bestPoint = keypoint2;
                    }
                }
            }
            ExtendedPoint *point1 = new ExtendedPoint(keypoint, frame1);
            frame1->keypoints.push_back(point1);
            ExtendedPoint *point2 = new ExtendedPoint(bestPoint, frame2);
            frame2->keypoints.push_back(point2);
            KeyPointTraceProcessor::checkAndAddToTrace(point1, point2);
            cout << "j'ai ajoute un point a la trace"<< endl;
        }
    }
}
