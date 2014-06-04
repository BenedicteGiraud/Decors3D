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

	Scalar colorFloat;
	Scalar color;
	Video* video;

	std::map<int, ExtendedPoint*> points;
	enum Type { scene, object, unknown};
	Type type;

	KalmanFilter kalmanFilter;

	ExtendedPoint* filter(Frame* frame);
	vector<ExtendedPoint*> filterAll(Frame* frame);

	KeyPoint* filterKeyPoint(Frame* frame);
	vector<KeyPoint> filterAllKeyPoints(Frame* frame);

	void addOrReplacePoint(ExtendedPoint* point);
	ExtendedPoint* firstPoint();
	ExtendedPoint* lastPoint();
};

#endif /* POINTTRACE_H_ */
