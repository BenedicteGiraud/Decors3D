/*
 * ResizeFrameAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef RESIZEFRAMEANNOTATOR_H_
#define RESIZEFRAMEANNOTATOR_H_

#include "FrameAnnotator.h"

class ResizeFrameAnnotator : public FrameAnnotator {
private:
	double factor;
public:
	ResizeFrameAnnotator();
	virtual ~ResizeFrameAnnotator();

	void setFactor(double factor);

	virtual void annotate(Video* video, Frame* frame, cv::Mat* image);
};

#endif /* RESIZEFRAMEANNOTATOR_H_ */
