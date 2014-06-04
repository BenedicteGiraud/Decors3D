/*
 * FrameProcessor.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef FRAMEPROCESSOR_H_
#define FRAMEPROCESSOR_H_

#include <cv.h>

#include "processor/ProcessorCallback.h"

class Video;
class Frame;

class ProcessorCallback;

class FrameProcessor : public ProcessorCallback {
public:
	FrameProcessor();
	virtual ~FrameProcessor();

	/**
	 * Process a frame, to detect sth, or to change the visualisation
	 * @param video
	 * @param frame
	 * @param image original frame or annotated frame of the visualization chain
	 */
	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback) = 0;

	virtual void processStart(Video* video);
	virtual void processEnd(Video* video);

	virtual cv::Point2f getOutputImageCoordinates(cv::Point2f inputCoordinates);
	virtual cv::Point2f getInputImageCoordinates(cv::Point2f outputCoordinates);
};

#endif /* FRAMEPROCESSOR_H_ */
