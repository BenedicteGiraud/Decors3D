/*
 * DoubleFrameProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef DOUBLEFRAMEPROCESSOR_H_
#define DOUBLEFRAMEPROCESSOR_H_

class Video;
class Frame;

class DoubleFrameProcessor {
public:
	DoubleFrameProcessor();
	virtual ~DoubleFrameProcessor();

	virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) = 0;

	virtual void processStart(Video* video, Frame* firstFrame);
	virtual void processEnd(Video* video, Frame* lastFrame);
};

#endif /* DOUBLEFRAMEPROCESSOR_H_ */
