﻿#include "CannyFlowTrace.h"

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

Video * CannyFlowTrace::getCannyVideo(){
    return &cannyVideo;
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

void CannyFlowTrace::processStart(Video* video, Frame* firstFrame){
    Mat cannyMat;
    cannyMat = getCannyPoints(firstFrame);

    // to show the cannyVideo
    Mat threeChannel;
    Mat in[] = {cannyMat, cannyMat, cannyMat};
    merge(in, 3, threeChannel);
    cannyVideo<<threeChannel;

    for(int row=0; row<firstFrame->image.rows; row++) {
        for(int col=0; col<firstFrame->image.cols; col++) {
            // I am a Canny Point
            if(cannyMat.at<uchar>(row, col) > 200) {

                Point2f keypoint = Point2f(col, row);
                Mat descr;
                KeyPointProcessor::extractPatchDescriptor(firstFrame->image, descr, keypoint);

                ExtendedPoint *point1 = new ExtendedPoint(keypoint, firstFrame);
                point1->descriptor = descr;
                firstFrame->keypoints.push_back(point1);

                int sizer = min(5, cannyMat.rows-row);
                int sizec = min(5, cannyMat.cols-col);
                cannyMat(Rect(col, row, sizec, sizer)) = Scalar(0);
            }
        }
    }
}

void CannyFlowTrace::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2){
    Mat cannyMat2;
    cannyMat2 = getCannyPoints(frame2);

    // to show the cannyVideo
    Mat threeChannel;
    Mat in[] = {cannyMat2, cannyMat2, cannyMat2};
    merge(in, 3, threeChannel);
    cannyVideo<<threeChannel;

    // boucle for with a thresh : Thresh in order to keep only points which have a value bigger than 200
    for (ExtendedPoint *point : frame1->keypoints) {
        // this is a canny point : we look for the keypoint in the canypoints
        Mat descr1 = point->descriptor;

        double distMin = DBL_MAX;
        Point2f bestPoint ;
        Mat bestDescr;
        // Compare this keypoint to thoses of the next frame
        for (int row2 = 0; row2 < frame2->image.rows; row2++){
            for (int col2 = 0; col2 < frame2->image.cols; col2++) {
                // it is not a canny point : we do not search a keypoint in thoses values
                if (cannyMat2.at<uchar>(row2, col2) < 250) {
                    continue;
                }
                // this is a canny point : we look for the keypoint in the canypoints
                Point2f keypoint2 = Point2f(col2, row2);
                Mat descr2;
                KeyPointProcessor::extractPatchDescriptor(frame2->image, descr2, keypoint2);
                double dist = KeyPointProcessor::descriptorDistance(descr1, descr2);
                if (dist < distMin) {
                    distMin = dist;
                    bestPoint = keypoint2;
                    bestDescr = descr2;
                }
            }
        }
        ExtendedPoint *point2 = new ExtendedPoint(bestPoint, frame2);
        point2->descriptor = bestDescr;
        frame2->keypoints.push_back(point2);
        KeyPointTraceProcessor::checkAndAddToTrace(point, point2);

        cout << "KeyPoints number = " << frame1->keypoints.size() << " **" << endl;
    }
}