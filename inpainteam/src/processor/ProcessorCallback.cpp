/*
 * ProcessorCallback.cpp
 *
 *  Created on: 2014/06/04
 *      Author: tr
 */

#include <cv.h>

#include "ProcessorCallback.h"

using namespace cv;

ProcessorCallback* ProcessorCallback::standard = NULL;

ProcessorCallback::ProcessorCallback() {
	// TODO Auto-generated constructor stub

}

ProcessorCallback::~ProcessorCallback() {
	// TODO Auto-generated destructor stub
}


Point2f ProcessorCallback::getOutputImageCoordinates(cv::Point2f inputCoordinates) {
	return inputCoordinates;
}

Point2f ProcessorCallback::getInputImageCoordinates(cv::Point2f outputCoordinates) {
	return outputCoordinates;
}

ProcessorCallback* ProcessorCallback::getDefault() {
	if(ProcessorCallback::standard == NULL) {
		ProcessorCallback::standard = new ProcessorCallback();
	}
	return ProcessorCallback::standard;
}
