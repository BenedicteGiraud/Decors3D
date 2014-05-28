/*
 * FrameAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef FRAMEANNOTATOR_H_
#define FRAMEANNOTATOR_H_

#include "Video.h"
#include "Frame.h"

class Video;

class FrameAnnotator {
public:
	FrameAnnotator();
	virtual ~FrameAnnotator();

	virtual void annotate(Video* video, Frame* frame, cv::Mat* image) = 0;
};

#endif /* FRAMEANNOTATOR_H_ */
