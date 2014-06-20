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

    static bool checkAndAddToTrace(ExtendedPoint *ep1, ExtendedPoint *ep2);

	virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);

	static double getSimilarityMeasure(ExtendedPoint *p1, ExtendedPoint *p2);
};

#endif /* KEYPOINTTRACEPROCESSOR_H_ */
