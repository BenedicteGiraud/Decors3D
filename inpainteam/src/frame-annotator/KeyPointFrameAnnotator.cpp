/*
 * KeyPointFrameAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include "KeyPointFrameAnnotator.h"
#include <cv.h>

using namespace cv;

KeyPointFrameAnnotator::KeyPointFrameAnnotator() {


}

KeyPointFrameAnnotator::~KeyPointFrameAnnotator() {

}

void KeyPointFrameAnnotator::annotate(Video* video, Frame* frame, cv::Mat* image) {
	Mat annotatedImage;
	if(frame->keypoints.size() == 0) {
		frame->detectKeyPoints();
	}
	drawKeypoints(*image, frame->keypoints, *image, Scalar(255,150,0), DrawMatchesFlags::DRAW_OVER_OUTIMG);
}
