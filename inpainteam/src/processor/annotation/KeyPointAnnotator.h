/*
 * KeyPointAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef KEYPOINTAnnotator_H_
#define KEYPOINTAnnotator_H_

#include "processor/FrameProcessor.h"

class KeyPointAnnotator : public FrameProcessor {
public:
	KeyPointAnnotator();
	virtual ~KeyPointAnnotator();

	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback);
};

#endif /* KEYPOINTAnnotator_H_ */
