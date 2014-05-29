/*
 * ExtendedPoint.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include "Video.h"
#include "Frame.h"
#include "PointTrace.h"

#include "ExtendedPoint.h"

ExtendedPoint::ExtendedPoint(cv::KeyPoint keypoint, Frame* frame) {
	this->keypoint = keypoint;
	this->frame = frame;
	this->trace = NULL;
}

ExtendedPoint::~ExtendedPoint() {

}

PointTrace* ExtendedPoint::getOrCreate() {
	if(this->trace == NULL) {
		this->trace = new PointTrace(this->frame->video);
		this->trace->points.push_back(this);
		this->frame->video->pointTraces.push_back(this->trace);
	}
	return this->trace;
}
