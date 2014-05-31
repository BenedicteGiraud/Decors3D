/*
 * FlowTraceProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef FlowTraceProcessor_H_
#define FlowTraceProcessor_H_

#include "processor/DoubleFrameProcessor.h"

class Video;
class Frame;

class FlowTraceProcessor : public DoubleFrameProcessor {
public:
	FlowTraceProcessor();
	virtual ~FlowTraceProcessor();

	virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);
};

#endif /* FlowTraceProcessor_H_ */
