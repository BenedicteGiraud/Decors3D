/*
 * KeyPointProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef KEYPOINTPROCESSOR_H_
#define KEYPOINTPROCESSOR_H_

#include "processor/FrameProcessor.h"

class KeyPointProcessor : public FrameProcessor {
public:
	KeyPointProcessor();
	virtual ~KeyPointProcessor();

	void processFrame(Video* video, Frame* frame, cv::Mat* image);
};

#endif /* KEYPOINTPROCESSOR_H_ */
