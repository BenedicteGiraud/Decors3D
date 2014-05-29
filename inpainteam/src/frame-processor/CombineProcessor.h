/*
 * CombineProcessor.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef CombineProcessor_H_
#define CombineProcessor_H_

#include "FrameProcessor.h"

class Video;
class Frame;

class CombineProcessor : public FrameProcessor {
public:
	CombineProcessor();
	virtual ~CombineProcessor();

	virtual void processFrame(Video* video, Frame* frame);

	virtual void processStart(Video* video);
	virtual void processEnd(Video* video);
};

#endif /* CombineProcessor_H_ */
