/*
 * Frame.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include "Frame.h"

using namespace cv;
#include <opencv2/nonfree/features2d.hpp>

Frame::Frame(Mat image) {
	this->image = image;
}

Frame::~Frame() {

}

void Frame::detectKeyPoints() {

	int minHessian;
	SurfFeatureDetector detector(minHessian);

	detector.detect(image, keypoints);
}
