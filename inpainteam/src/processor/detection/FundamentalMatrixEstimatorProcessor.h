/*
 * HomographyEstimatorProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef FundamentalMatrixESTIMATORPROCESSOR_H_
#define FundamentalMatrixESTIMATORPROCESSOR_H_

#include "processor/DoubleFrameProcessor.h"

class Video;
class Frame;

class FundamentalMatrixEstimatorProcessor : public DoubleFrameProcessor {
private:
	vector<PointTrace*> traces;

public:
	FundamentalMatrixEstimatorProcessor();
	virtual ~FundamentalMatrixEstimatorProcessor();

	virtual void processStart(Video* video, Frame* firstframe);
	virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);
};

#endif /* HOMOGRAPHYESTIMATORPROCESSOR_H_ */
