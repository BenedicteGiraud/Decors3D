/*
 * SceneTraceClassifierProcessor.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef SceneTraceClassifierProcessor_H_
#define SceneTraceClassifierProcessor_H_

#include "VideoProcessor.h"

class Video;
class Frame;

class SceneTraceClassifierProcessor : public VideoProcessor {
public:
	SceneTraceClassifierProcessor();
	virtual ~SceneTraceClassifierProcessor();

	virtual void process(Video* video);
};

#endif /* SceneTraceClassifierProcessor_H_ */
