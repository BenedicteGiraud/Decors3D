/*
 * ProcessorCallback.h
 *
 *  Created on: 2014/06/04
 *      Author: tr
 */

#ifndef PROCESSORCALLBACK_H_
#define PROCESSORCALLBACK_H_

#include <cv.h>

class ProcessorCallback;

class ProcessorCallback {
public:
	ProcessorCallback();
	virtual ~ProcessorCallback();

	static ProcessorCallback* standard;
	static ProcessorCallback* getDefault();

	virtual cv::Point2f getOutputImageCoordinates(cv::Point2f inputCoordinates);
	virtual cv::Point2f getInputImageCoordinates(cv::Point2f outputCoordinates);

	virtual void refreshGui();
};

#endif /* PROCESSORCALLBACK_H_ */
