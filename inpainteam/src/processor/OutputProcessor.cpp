/*
 * OutputProcessor.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "entities/Video.h"
#include "entities/Frame.h"

#include "OutputProcessor.h"

using namespace cv;

OutputProcessor::OutputProcessor() {
	output = NULL;
	index = 0;
}

OutputProcessor::~OutputProcessor() {

}

void OutputProcessor::setOutput(Video* video) {
	output = video;
	index = 0;
}

void OutputProcessor::processFrame(Video* video, Frame* frame, cv::Mat* image) {
	output->frames.push_back(new Frame(image->clone(), output, index++));
}
