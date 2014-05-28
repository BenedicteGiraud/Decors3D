/*
 * KeyPointFrameAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef KEYPOINTFRAMEANNOTATOR_H_
#define KEYPOINTFRAMEANNOTATOR_H_

#include "FrameAnnotator.h"

class KeyPointFrameAnnotator : public FrameAnnotator {
public:
	KeyPointFrameAnnotator();
	virtual ~KeyPointFrameAnnotator();

	virtual void annotate(Video* video, Frame* frame, cv::Mat* image);
};

#endif /* KEYPOINTFRAMEANNOTATOR_H_ */
