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
public:
	FundamentalMatrixEstimatorProcessor();
	virtual ~FundamentalMatrixEstimatorProcessor();

	virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);
};

#endif /* HOMOGRAPHYESTIMATORPROCESSOR_H_ */
