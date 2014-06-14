/*
 * KeyPointProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef KEYPOINTPROCESSOR_H_
#define KEYPOINTPROCESSOR_H_

#include <cv.h>

#include "entities/ExtendedPoint.h"

#include "processor/FrameProcessor.h"

class KeyPointProcessor : public FrameProcessor {
public:
	KeyPointProcessor();
	virtual ~KeyPointProcessor();

	void processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback);

	static void extractPatchDescriptor(cv::Mat image, cv::Mat &descriptor, cv::Point2f keypoint, int halfSideLength=5, float scale=2);
	static double descriptorDistance(cv::Mat desc1, cv::Mat desc2);
	static void addKeypoint(Frame* frame, cv::Point2f point, cv::Mat descriptor, ExtendedPoint::Detector detector);
};

#endif /* KEYPOINTPROCESSOR_H_ */
