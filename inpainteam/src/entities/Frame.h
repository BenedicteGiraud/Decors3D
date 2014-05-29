/*
 * Frame.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <cv.h>

#include "entities/ExtendedPoint.h"

class Frame {
public:
	cv::Mat image;
	std::vector<ExtendedPoint> keypoints;

	Frame(cv::Mat image);
	virtual ~Frame();

	void detectKeyPoints();
};

#endif /* FRAME_H_ */
