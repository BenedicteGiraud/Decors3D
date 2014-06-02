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
class Frame;
class ExtendedPoint;

class PointTrace {
public:
	PointTrace(Video* video);
	virtual ~PointTrace();

	Scalar color;
	Video* video;

	std::map<Frame*, ExtendedPoint*> points;

	ExtendedPoint* filter(Frame* frame);
	vector<ExtendedPoint*> filterAll(Frame* frame);

	KeyPoint* filterKeyPoint(Frame* frame);
	vector<KeyPoint> filterAllKeyPoints(Frame* frame);

	void addOrReplacePoint(ExtendedPoint* point);
	ExtendedPoint* firstPoint();
	ExtendedPoint* lastPoint();
};

#endif /* POINTTRACE_H_ */
