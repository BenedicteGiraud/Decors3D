/*
 * TraceInterpolationProcessor.h
 *
 *  Created on: 2014/06/02
 *      Author: tr
 */

#ifndef TRACEINTERPOLATIONPROCESSOR_H_
#define TRACEINTERPOLATIONPROCESSOR_H_

#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/flann/kdtree_index.h>

#include "processor/FrameProcessor.h"

using namespace cv;

class TraceInterpolationProcessor: public FrameProcessor {
public:
	TraceInterpolationProcessor();
	virtual ~TraceInterpolationProcessor();

	cv::Mat result;
	cv::Mat count;

	virtual void processStart(Video *video);
	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image);

	cv::Mat getImage();
	cv::Mat getCountImage();
};

#endif /* TRACEINTERPOLATIONPROCESSOR_H_ */
