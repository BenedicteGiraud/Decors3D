/*
 * ExtendedPoint.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include <cv.h>

#include "Video.h"
#include "Frame.h"
#include "PointTrace.h"

#include "ExtendedPoint.h"

ExtendedPoint::ExtendedPoint(Point2f point, Frame* frame) {
	this->coordinates = point;
	this->frame = frame;
	this->trace = NULL;
	this->detector = unknown;
}

ExtendedPoint::ExtendedPoint(cv::KeyPoint keypoint, Frame* frame) {
	this->coordinates = keypoint.pt;
	this->keypoint = keypoint;
	this->frame = frame;
	this->trace = NULL;
	this->detector = unknown;
}

ExtendedPoint::~ExtendedPoint() {

}

PointTrace* ExtendedPoint::getOrCreate() {
	if(this->trace == NULL) {
		this->trace = new PointTrace(this->frame->video);
		this->trace->addOrReplacePoint(this);
		this->frame->video->pointTraces.push_back(this->trace);
	}
	return this->trace;
}

