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
	ResizeAnnotator(double factor);
	virtual ~ResizeAnnotator();

	void setFactor(double factor);

	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback);

	virtual cv::Point2f getOutputImageCoordinates(cv::Point2f inputCoordinates);
	virtual cv::Point2f getInputImageCoordinates(cv::Point2f outputCoordinates);
};

#endif /* RESIZEAnnotator_H_ */
