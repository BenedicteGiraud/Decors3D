/*
 * TraceFrameAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef TraceFrameAnnotator_H_
#define TraceFrameAnnotator_H_

#include "FrameAnnotator.h"

class TraceFrameAnnotator : public FrameAnnotator {
public:
	TraceFrameAnnotator();
	virtual ~TraceFrameAnnotator();

	virtual void annotate(Video* video, Frame* frame, cv::Mat* image);
};

#endif /* TraceFrameAnnotator_H_ */
