/*
 * PointTrace.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include <time.h>
#include "entities/Frame.h"
#include "PointTrace.h"

using namespace std;

PointTrace::PointTrace(Video* video) {
	int r = 0.5+0.5*rand();
	int g = 0.5+0.5*rand();
	int b = 0.5+0.5*rand();
	colorFloat = Scalar(r / RAND_MAX, g / RAND_MAX, b / RAND_MAX);
	color = Scalar(255*r, 255*g, 255*b);
	this->video = video;
	type = Type::unknown;
	first = NULL;
	last = NULL;
}

PointTrace::~PointTrace() {

}

ExtendedPoint* PointTrace::filter(int index) {
	auto it = this->points.find(index);
	if(it != this->points.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

ExtendedPoint* PointTrace::filter(Frame* frame) {
	if(frame == NULL) {
		return NULL;
	}
	return filter(frame->index);
}

// TODO use unordered map Frame -> ExtendedPoint
KeyPoint* PointTrace::filterKeyPoint(Frame* frame) {
	ExtendedPoint* ep = filter(frame);
	if(ep != NULL) {
		return &ep->keypoint;
	}
	else {
		return NULL;
	}
}

// TODO: use unordered map Frame -> ExtendedPoint
void PointTrace::addOrReplacePoint(ExtendedPoint* point) {
	ExtendedPoint* old = filter(point->frame);
	if(old != NULL) {
		old->trace = NULL;
	}
	point->trace = this;
	this->points.insert(pair<int, ExtendedPoint*>(point->frame->index, point));

	if(first == NULL || point->frame->index < first->frame->index) {
		first = point;
	}
	if(last == NULL || point->frame->index > last->frame->index) {
		last = point;
	}
}

// TODO: use map int -> ExtendedPoint
ExtendedPoint* PointTrace::firstPoint() {
	return first;
}

// TODO: use int -> ExtendedPoint
ExtendedPoint* PointTrace::lastPoint() {
	return last;
}
