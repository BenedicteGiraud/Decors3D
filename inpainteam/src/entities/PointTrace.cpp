/*
 * PointTrace.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include "PointTrace.h"
#include <time.h>

PointTrace::PointTrace() {
	srand((unsigned int)time(NULL));
	color = Scalar(255*rand(), 255*rand(), 255*rand());
}

PointTrace::~PointTrace() {

}

