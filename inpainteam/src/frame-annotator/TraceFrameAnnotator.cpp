/*
 * TraceFrameAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "TraceFrameAnnotator.h"
#include "entities/ExtendedPoint.h"

using namespace cv;

TraceFrameAnnotator::TraceFrameAnnotator() {

}

TraceFrameAnnotator::~TraceFrameAnnotator() {

}

void TraceFrameAnnotator::annotate(Video* video, Frame* frame, cv::Mat* image) {
	Mat annotatedImage;
	for(auto trace : video->traces) {
		if(trace.points.size() >= 2) {
			double distance = norm(trace.points.front().keypoint.pt - trace.points.back().keypoint.pt);

			vector<KeyPoint> todraw;
			for(auto point : trace.points) {
				todraw.push_back(point.keypoint);
			}

			if(distance < 30)
				drawKeypoints(*image, todraw, *image, Scalar(0,0,0), DrawMatchesFlags::DRAW_OVER_OUTIMG);
			else
				drawKeypoints(*image, todraw, *image, trace.color, DrawMatchesFlags::DRAW_OVER_OUTIMG);

		}
	}
}
