/*
 * FrameProcessor.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef FRAMEPROCESSOR_H_
#define FRAMEPROCESSOR_H_

class Video;
class Frame;

class FrameProcessor {
public:
	FrameProcessor();
	virtual ~FrameProcessor();

	virtual void processFrame(Video* video, Frame* frame) = 0;

	virtual void processStart();
	virtual void processEnd();
};

#endif /* FRAMEPROCESSOR_H_ */
