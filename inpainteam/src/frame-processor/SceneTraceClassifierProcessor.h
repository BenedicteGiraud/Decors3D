/*
 * SceneTraceClassifierProcessor.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef SceneTraceClassifierProcessor_H_
#define SceneTraceClassifierProcessor_H_

#include "FrameProcessor.h"

class Video;
class Frame;

class SceneTraceClassifierProcessor : public FrameProcessor {
public:
	SceneTraceClassifierProcessor();
	virtual ~SceneTraceClassifierProcessor();

	virtual void processFrame(Video* video, Frame* frame);

	virtual void processStart(Video* video);
	virtual void processEnd(Video* video);
};

#endif /* SceneTraceClassifierProcessor_H_ */
