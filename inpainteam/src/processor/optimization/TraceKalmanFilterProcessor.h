/*
 * TraceKalmanFilterProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef TraceKalmanFilterProcessor_H_
#define TraceKalmanFilterProcessor_H_

#include "processor/DoubleFrameProcessor.h"

class Video;
class Frame;

class TraceKalmanFilterProcessor : public DoubleFrameProcessor {
public:
	TraceKalmanFilterProcessor();
	virtual ~TraceKalmanFilterProcessor();

	virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);

	virtual void processStart(Video* video, Frame* firstFrame);
};

#endif /* TraceKalmanFilterProcessor_H_ */
