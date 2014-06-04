/*
 * PipelineProcessor.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "processor/PipelineProcessor.h"

#include "PipelineProcessor.h"

using namespace cv;

PipelineProcessor::PipelineProcessor() {
	factor = 1;
}

PipelineProcessor::~PipelineProcessor() {

}

void PipelineProcessor::add(FrameProcessor* processor) {
	annotators.push_back(processor);
	if(callbacks.size() > 0) {
		PipelineProcessorCallback* callback = new PipelineProcessorCallback();
		callback->preceedingProcessorCallback = callbacks.back();
		callback->callback = processor;
		callbacks.push_back(callback);
	}
	else {
		callbacks.push_back(processor);
	}
}

void PipelineProcessor::processStart(Video* video) {
	vector<FrameProcessor*>::iterator it = annotators.begin();
	for(; it != annotators.end(); it++) {
		(*it)->processStart(video);
	}
}

void PipelineProcessor::processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback) {
	// TODO: deal with callback
	vector<FrameProcessor*>::iterator it = annotators.begin();
	vector<ProcessorCallback*>::iterator itCallback = callbacks.begin();
	for(; it != annotators.end(), itCallback != callbacks.end(); it++, itCallback++) {
		(*it)->processFrame(video, frame, image, *itCallback);
	}

}


void PipelineProcessor::processEnd(Video* video) {
	vector<FrameProcessor*>::iterator it = annotators.begin();
	for(; it != annotators.end(); it++) {
		(*it)->processEnd(video);
	}
}

Point2f PipelineProcessorCallback::getOutputImageCoordinates(Point2f inputCoordinates) {
	return callback->getOutputImageCoordinates(preceedingProcessorCallback->getOutputImageCoordinates(inputCoordinates));
}

Point2f PipelineProcessorCallback::getInputImageCoordinates(Point2f outputCoordinates) {
	return preceedingProcessorCallback->getInputImageCoordinates(callback->getInputImageCoordinates(outputCoordinates));
}
