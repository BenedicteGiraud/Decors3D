/*
 * PipelineFrameAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>
#include "PipelineFrameAnnotator.h"

using namespace cv;

PipelineFrameAnnotator::PipelineFrameAnnotator() {
	factor = 1;
}

PipelineFrameAnnotator::~PipelineFrameAnnotator() {

}

void PipelineFrameAnnotator::add(FrameAnnotator* annotator) {
	annotators.push_back(annotator);
}

void PipelineFrameAnnotator::annotate(Video* video, Frame* frame, cv::Mat* image) {
	vector<FrameAnnotator*>::iterator it = annotators.begin();
	for(; it != annotators.end(); it++) {
		(*it)->annotate(video, frame, image);
	}

}
