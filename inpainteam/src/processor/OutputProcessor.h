/*
 * OutputProcessor.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef OutputProcessor_H_
#define OutputProcessor_H_

#include "processor/FrameProcessor.h"

class OutputProcessor : public FrameProcessor {
private:
	Video* output;
	int index;

public:
	OutputProcessor();
	virtual ~OutputProcessor();

	void setOutput(Video* output);

	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image);
};

#endif /* OutputProcessor_H_ */
