/*
 * FrameProcessor.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef FRAMEPROCESSOR_H_
#define FRAMEPROCESSOR_H_

#include <cv.h>

class Video;
class Frame;

class FrameProcessor {
public:
	FrameProcessor();
	virtual ~FrameProcessor();

	/**
	 * Process a frame, to detect sth, or to change the visualisation
	 * @param video
	 * @param frame
	 * @param image original frame or annotated frame of the visualization chain
	 */
	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image) = 0;

	virtual void processStart(Video* video);
	virtual void processEnd(Video* video);
};

#endif /* FRAMEPROCESSOR_H_ */
