/*
 * ResizeAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef RESIZEAnnotator_H_
#define RESIZEAnnotator_H_

#include "processor/FrameProcessor.h"

class ResizeAnnotator : public FrameProcessor {
private:
	double factor;
public:
	ResizeAnnotator();
	virtual ~ResizeAnnotator();

	void setFactor(double factor);

	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image);
};

#endif /* RESIZEAnnotator_H_ */
