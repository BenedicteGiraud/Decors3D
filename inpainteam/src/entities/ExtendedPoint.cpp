/*
 * ExtendedPoint.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include "ExtendedPoint.h"

ExtendedPoint::ExtendedPoint(cv::KeyPoint keypoint) {
	this->keypoint = keypoint;
}

ExtendedPoint::~ExtendedPoint() {

}
