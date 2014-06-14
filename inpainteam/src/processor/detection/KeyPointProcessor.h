/*
 * KeyPointProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef KEYPOINTPROCESSOR_H_
#define KEYPOINTPROCESSOR_H_

#include <cv.h>

#include "processor/FrameProcessor.h"

class KeyPointProcessor : public FrameProcessor {
public:
	KeyPointProcessor();
	virtual ~KeyPointProcessor();

	void processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback);

	static double descriptorDistance(cv::Mat desc1, cv::Mat desc2);
};

#endif /* KEYPOINTPROCESSOR_H_ */
