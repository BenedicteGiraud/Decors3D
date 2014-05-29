/*
 * TraceFrameAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "TraceFrameAnnotator.h"
#include "entities/ExtendedPoint.h"
#include "entities/Video.h"

using namespace cv;

TraceFrameAnnotator::TraceFrameAnnotator() {

}

TraceFrameAnnotator::~TraceFrameAnnotator() {

}

void TraceFrameAnnotator::annotate(Video* video, Frame* frame, cv::Mat* image) {
	Mat annotatedImage;
	int i = -1;
	for(auto trace : video->pointTraces) {
		i++;

		if(trace->points.size() >= 5) {
			//if(i > 30) break;
			double distance = norm((*trace->points.front()).keypoint.pt - (*trace->points.back()).keypoint.pt);

			vector<KeyPoint> todraw;
			for(auto point : trace->points) {
				if(point->frame == frame) {
					todraw.push_back(point->keypoint);
				}
			}

			if(distance < 10)
				drawKeypoints(*image, todraw, *image, trace->color, DrawMatchesFlags::DRAW_OVER_OUTIMG);
			else {
				Scalar color = trace->color;
				for(int i=0; i<4; i++) {
					color.val[i] *= 0.5;
				}
				//drawKeypoints(*image, todraw, *image, trace->color, DrawMatchesFlags::DRAW_OVER_OUTIMG);
				for(auto point : todraw) {
					Point p1 = point.pt, p2 = point.pt;
					int diff = 2;
					p1.x -= diff; p1.y -= diff;
					p2.x += diff; p2.y += diff;
					rectangle(*image, p1, p2, trace->color);
					break;
				}
			}

		}
	}
}
