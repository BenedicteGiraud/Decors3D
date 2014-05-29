/*
 * KeyPointProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef KEYPOINTPROCESSOR_H_
#define KEYPOINTPROCESSOR_H_

#include "frame-processor/FrameProcessor.h"

class KeyPointProcessor : public FrameProcessor {
public:
	KeyPointProcessor();
	virtual ~KeyPointProcessor();

	void processFrame(Video* video, Frame* frame);
};

#endif /* KEYPOINTPROCESSOR_H_ */