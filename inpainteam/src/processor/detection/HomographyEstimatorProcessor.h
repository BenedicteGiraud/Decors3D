/*
 * HomographyEstimatorProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef HOMOGRAPHYESTIMATORPROCESSOR_H_
#define HOMOGRAPHYESTIMATORPROCESSOR_H_

#include "processor/DoubleFrameProcessor.h"

class Video;
class Frame;

class HomographyEstimatorProcessor : public DoubleFrameProcessor {

	vector<PointTrace*> traces;

public:
	HomographyEstimatorProcessor();
	virtual ~HomographyEstimatorProcessor();

	virtual void processStart(Video* video, Frame* firstframe);
	virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);
};

#endif /* HOMOGRAPHYESTIMATORPROCESSOR_H_ */
