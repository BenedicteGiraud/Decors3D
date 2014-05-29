/*
 * KeyPointFrameAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "KeyPointFrameAnnotator.h"
#include "entities/ExtendedPoint.h"
#include "entities/Frame.h"

using namespace cv;

KeyPointFrameAnnotator::KeyPointFrameAnnotator() {

}

KeyPointFrameAnnotator::~KeyPointFrameAnnotator() {

}

void KeyPointFrameAnnotator::annotate(Video* video, Frame* frame, cv::Mat* image) {
	Mat annotatedImage;
	vector<ExtendedPoint>::iterator it = frame->keypoints.begin();
	for(; it != frame->keypoints.end(); it++) {
		vector<KeyPoint> todraw;
		todraw.push_back(it->keypoint);
		drawKeypoints(*image, todraw, *image, Scalar(255,150,0), DrawMatchesFlags::DRAW_OVER_OUTIMG);
	}
}
