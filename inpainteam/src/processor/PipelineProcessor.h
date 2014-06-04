/*
 * PipelineProcessor.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef PipelineProcessor_H_
#define PipelineProcessor_H_

#include <vector>

#include "processor/FrameProcessor.h"

using namespace std;

class PipelineProcessor : public FrameProcessor {
private:
	double factor;
	vector<FrameProcessor*> annotators;
	vector<ProcessorCallback*> callbacks;

public:
	PipelineProcessor();
	virtual ~PipelineProcessor();

	void add(FrameProcessor *processor);

	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback);

	virtual void processStart(Video* video);
	virtual void processEnd(Video* video);
};

class PipelineProcessorCallback : public ProcessorCallback {
private:
	friend PipelineProcessor;
	ProcessorCallback* preceedingProcessorCallback;
	ProcessorCallback* callback;

public:
	virtual cv::Point2f getOutputImageCoordinates(cv::Point2f inputCoordinates);
	virtual cv::Point2f getInputImageCoordinates(cv::Point2f outputCoordinates);
};

#endif /* PipelineProcessor_H_ */
