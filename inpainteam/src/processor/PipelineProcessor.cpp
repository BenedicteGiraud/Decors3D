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
	PipelineProcessorCallback* callback = new PipelineProcessorCallback();

	if(callbacks.size() > 0) {
		callback->preceedingProcessorCallback = callbacks.back();
	}
	else {
		callback->preceedingProcessorCallback = ProcessorCallback::getDefault();
	}
	callback->callback = processor;
	callbacks.push_back(callback);
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
	vector<PipelineProcessorCallback*>::iterator itCallback = callbacks.begin();
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

Point2f PipelineProcessor::getOutputImageCoordinates(Point2f inputCoordinates) {
	return callbacks.back()->getOutputImageCoordinates(inputCoordinates);
}

Point2f PipelineProcessor::getInputImageCoordinates(Point2f outputCoordinates) {
	return callbacks.back()->getInputImageCoordinates(outputCoordinates);
}

void PipelineProcessor::mouseEventCallback(int event, int x, int y, int flags, ProcessorCallback* callback) {
	// TODO: deal with callback
	vector<FrameProcessor*>::iterator it = annotators.begin();
	vector<PipelineProcessorCallback*>::iterator itCallback = callbacks.begin();
	bool refreshGuiCalled = false;;
	for(; it != annotators.end(), itCallback != callbacks.end(); it++, itCallback++) {
		// quick'n'dirty
		EventListener* listener = dynamic_cast<EventListener*>(*it);
		if(listener != NULL) {
			(*itCallback)->refreshGuiCalled = false;
			listener->mouseEventCallback(event, x, y, flags, *itCallback);
			refreshGuiCalled |= (*itCallback)->refreshGuiCalled;
		}
	}
	if(refreshGuiCalled) {
		callback->refreshGui();
	}
}

Point2f PipelineProcessorCallback::getOutputImageCoordinates(Point2f inputCoordinates) {
	return callback->getOutputImageCoordinates(preceedingProcessorCallback->getOutputImageCoordinates(inputCoordinates));
}

Point2f PipelineProcessorCallback::getInputImageCoordinates(Point2f outputCoordinates) {
	return preceedingProcessorCallback->getInputImageCoordinates(callback->getInputImageCoordinates(outputCoordinates));
}

void PipelineProcessorCallback::refreshGui() {
	refreshGuiCalled = true;
}
