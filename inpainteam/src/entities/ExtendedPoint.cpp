/*
 * ExtendedPoint.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include "ExtendedPoint.h"

ExtendedPoint::ExtendedPoint(cv::KeyPoint keypoint, Frame* frame) {
	this->keypoint = keypoint;
	this->frame = frame;
}

ExtendedPoint::~ExtendedPoint() {

}
