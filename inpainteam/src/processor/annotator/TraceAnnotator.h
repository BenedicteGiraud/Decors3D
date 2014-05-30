/*
 * TraceAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef TraceAnnotator_H_
#define TraceAnnotator_H_

#include "processor/FrameProcessor.h"

class TraceAnnotator : public FrameProcessor {
public:
	TraceAnnotator();
	virtual ~TraceAnnotator();

	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image);
};

#endif /* TraceAnnotator_H_ */
