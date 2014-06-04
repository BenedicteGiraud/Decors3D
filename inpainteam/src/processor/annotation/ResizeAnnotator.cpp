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

ResizeAnnotator::ResizeAnnotator(double factor) {
	this->factor = factor;
}

ResizeAnnotator::~ResizeAnnotator() {

}

void ResizeAnnotator::setFactor(double factor) {
	this->factor = factor;
}

void ResizeAnnotator::processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback) {
	resize(*image, *image, Size(), factor, factor, INTER_LINEAR);
}

Point2f ResizeAnnotator::getOutputImageCoordinates(Point2f inputCoordinates) {
	Point2f result = inputCoordinates;
	result.x *= factor;
	result.y *= factor;
	return result;
}

Point2f ResizeAnnotator::getInputImageCoordinates(Point2f outputCoordinates) {
	Point2f result = outputCoordinates;
	result.x /= factor;
	result.y /= factor;
	return result;
}
