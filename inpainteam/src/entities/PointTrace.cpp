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
	color = Scalar(255*rand(), 255*rand(), 255*rand());
	this->video = video;
}

PointTrace::~PointTrace() {

}

ExtendedPoint* PointTrace::filter(Frame* frame) {
	auto it = this->points.find(frame);
	if(it != this->points.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
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
	this->points.insert(pair<Frame*, ExtendedPoint*>(point->frame, point));
}

// TODO: use map int -> ExtendedPoint
ExtendedPoint* PointTrace::firstPoint() {
	auto it = points.begin();
	if(it != points.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

// TODO: use int -> ExtendedPoint
ExtendedPoint* PointTrace::lastPoint() {
	auto it = points.rbegin();
	if(it != points.rend()) {
		return it->second;
	}
	else {
		return NULL;
	}
}
