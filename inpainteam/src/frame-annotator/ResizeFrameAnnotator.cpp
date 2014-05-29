/*
 * ResizeFrameAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "ResizeFrameAnnotator.h"

using namespace cv;

ResizeFrameAnnotator::ResizeFrameAnnotator() {
	factor = 1;
}

ResizeFrameAnnotator::~ResizeFrameAnnotator() {

}

void ResizeFrameAnnotator::setFactor(double factor) {
	this->factor = factor;
}

void ResizeFrameAnnotator::annotate(Video* video, Frame* frame, cv::Mat* image) {
	resize(*image, *image, Size(), factor, factor, INTER_LINEAR);
}
