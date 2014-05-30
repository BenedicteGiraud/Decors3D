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

public:
	PipelineProcessor();
	virtual ~PipelineProcessor();

	void add(FrameProcessor *processor);

	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image);

	virtual void processStart(Video* video);
	virtual void processEnd(Video* video);
};

#endif /* PipelineProcessor_H_ */
