/*
 * KeyPointTraceProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef KEYPOINTTRACEPROCESSOR_H_
#define KEYPOINTTRACEPROCESSOR_H_

#include "processor/DoubleFrameProcessor.h"

class Video;
class Frame;

class KeyPointTraceProcessor : public DoubleFrameProcessor {
public:
	KeyPointTraceProcessor();
	virtual ~KeyPointTraceProcessor();

	virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);
};

#endif /* KEYPOINTTRACEPROCESSOR_H_ */
