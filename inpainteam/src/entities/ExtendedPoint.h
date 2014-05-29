/*
 * ExtendedPoint.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef EXTENDEDPOINT_H_
#define EXTENDEDPOINT_H_

#include <cv.h>

class Frame;
class PointTrace;

class ExtendedPoint {
public:
	ExtendedPoint(cv::KeyPoint keypoint, Frame* frame);
	virtual ~ExtendedPoint();

	Frame* frame;
	PointTrace* trace;

	cv::KeyPoint keypoint;
	cv::Mat descriptor;

	PointTrace* getOrCreate();
};

#endif /* EXTENDEDPOINT_H_ */
