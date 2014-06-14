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
	ExtendedPoint(cv::Point2f point, Frame* frame);
	ExtendedPoint(cv::KeyPoint keypoint, Frame* frame);
	virtual ~ExtendedPoint();

	Frame* frame;
	PointTrace* trace;

	cv::Point2f coordinates;

	cv::KeyPoint keypoint;
	cv::Mat descriptor;

	PointTrace* getOrCreate();

	enum Detector { unknown, SURF, GFTT, Canny, Harris, projected};
	Detector detector;
};

#endif /* EXTENDEDPOINT_H_ */
