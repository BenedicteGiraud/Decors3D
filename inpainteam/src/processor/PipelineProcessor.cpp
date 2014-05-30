/*
 * PipelineProcessor.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>
#include "PipelineProcessor.h"

using namespace cv;

PipelineProcessor::PipelineProcessor() {
	factor = 1;
}

PipelineProcessor::~PipelineProcessor() {

}

void PipelineProcessor::add(FrameProcessor* processor) {
	annotators.push_back(processor);
}

void PipelineProcessor::processStart(Video* video) {
	vector<FrameProcessor*>::iterator it = annotators.begin();
	for(; it != annotators.end(); it++) {
		(*it)->processStart(video);
	}
}

void PipelineProcessor::processFrame(Video* video, Frame* frame, cv::Mat* image) {
	vector<FrameProcessor*>::iterator it = annotators.begin();
	for(; it != annotators.end(); it++) {
		(*it)->processFrame(video, frame, image);
	}

}


void PipelineProcessor::processEnd(Video* video) {
	vector<FrameProcessor*>::iterator it = annotators.begin();
	for(; it != annotators.end(); it++) {
		(*it)->processEnd(video);
	}
}
