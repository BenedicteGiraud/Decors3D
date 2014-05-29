/*
 * PointTrace.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include <time.h>
#include "entities/Frame.h"
#include "PointTrace.h"

PointTrace::PointTrace(Video* video) {
	color = Scalar(255*rand(), 255*rand(), 255*rand());
	this->video = video;
}

PointTrace::~PointTrace() {

}


vector<KeyPoint> PointTrace::filterAll(Frame* frame) {
	vector<KeyPoint> result;
	for(auto point : points) {
		if(point->frame == frame) {
			result.push_back(point->keypoint);
		}
	}
	return result;
}

KeyPoint* PointTrace::filter(Frame* frame) {
	for(auto point : points) {
		if(point->frame == frame) {
			return &point->keypoint;
		}
	}

	return NULL;
}
