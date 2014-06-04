/*
 * KeyPointAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "KeyPointAnnotator.h"
#include "entities/ExtendedPoint.h"
#include "entities/Frame.h"

using namespace cv;

KeyPointAnnotator::KeyPointAnnotator() {

}

KeyPointAnnotator::~KeyPointAnnotator() {

}

void KeyPointAnnotator::processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback) {
	Mat annotatedImage;
	auto it = frame->keypoints.begin();
	for(; it != frame->keypoints.end(); it++) {
		vector<KeyPoint> todraw;
		todraw.push_back((*it)->keypoint);
		drawKeypoints(*image, todraw, *image, Scalar(255,150,0), DrawMatchesFlags::DRAW_OVER_OUTIMG);
	}
}
