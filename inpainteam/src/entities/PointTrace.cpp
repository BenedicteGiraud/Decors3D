/*
 * PointTrace.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include "PointTrace.h"
#include <time.h>

PointTrace::PointTrace(Video* video) {
	color = Scalar(255*rand(), 255*rand(), 255*rand());
	this->video = video;
}

PointTrace::~PointTrace() {

}

