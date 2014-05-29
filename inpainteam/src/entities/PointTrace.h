/*
 * PointTrace.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef POINTTRACE_H_
#define POINTTRACE_H_

#include <cv.h>

using namespace cv;

class Video;
class ExtendedPoint;

class PointTrace {
public:
	PointTrace(Video* video);
	virtual ~PointTrace();

	Scalar color;
	Video* video;

	vector<ExtendedPoint*> points;
};

#endif /* POINTTRACE_H_ */
