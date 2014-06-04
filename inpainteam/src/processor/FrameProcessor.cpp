/*
 * FrameProcessor.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "processor/FrameProcessor.h"

using namespace cv;

FrameProcessor::FrameProcessor() {

}

FrameProcessor::~FrameProcessor() {

}

void FrameProcessor::processStart(Video* video) {

}

void FrameProcessor::processEnd(Video* video) {

}

Point2f FrameProcessor::getOutputImageCoordinates(Point2f inputCoordinates) {
	return inputCoordinates;
}

Point2f FrameProcessor::getInputImageCoordinates(Point2f outputCoordinates) {
	return outputCoordinates;
}
