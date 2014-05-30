/*
 * ResizeAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "ResizeAnnotator.h"

using namespace cv;

ResizeAnnotator::ResizeAnnotator() {
	factor = 1;
}

ResizeAnnotator::~ResizeAnnotator() {

}

void ResizeAnnotator::setFactor(double factor) {
	this->factor = factor;
}

void ResizeAnnotator::processFrame(Video* video, Frame* frame, cv::Mat* image) {
	resize(*image, *image, Size(), factor, factor, INTER_LINEAR);
}
