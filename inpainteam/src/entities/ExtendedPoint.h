/*
 * ExtendedPoint.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef EXTENDEDPOINT_H_
#define EXTENDEDPOINT_H_

#include <cv.h>

class ExtendedPoint {
public:
	ExtendedPoint(cv::KeyPoint keypoint);
	virtual ~ExtendedPoint();

	cv::KeyPoint keypoint;
};

#endif /* EXTENDEDPOINT_H_ */
