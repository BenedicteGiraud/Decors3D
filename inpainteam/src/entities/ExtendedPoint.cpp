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


Point2f ExtendedPoint::applyHomography(Mat homography, Point2f point) {

	Mat p(3, 1, CV_64F);
	p.at<double>(0,0) = point.x;
	p.at<double>(1,0) = point.y;
	p.at<double>(2,0) = 1;

	Mat projected = homography * p;

	Point2f result;
	result.x = projected.at<double>(0,0) / projected.at<double>(2,0);
	result.y = projected.at<double>(1,0) / projected.at<double>(2,0);

	return result;
}
