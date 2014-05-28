/*
 * PipelineFrameAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef PipelineFrameAnnotator_H_
#define PipelineFrameAnnotator_H_

#include <vector>

#include "FrameAnnotator.h"

using namespace std;

class PipelineFrameAnnotator : public FrameAnnotator {
private:
	double factor;
	vector<FrameAnnotator*> annotators;

public:
	PipelineFrameAnnotator();
	virtual ~PipelineFrameAnnotator();

	void add(FrameAnnotator *annotator);

	virtual void annotate(Video* video, Frame* frame, cv::Mat* image);
};

#endif /* PipelineFrameAnnotator_H_ */
