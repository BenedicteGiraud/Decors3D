/*
 * TraceAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "TraceAnnotator.h"
#include "entities/ExtendedPoint.h"
#include "entities/Video.h"
#include "entities/Frame.h"

using namespace cv;
using namespace std;

TraceAnnotator::TraceAnnotator() {

}

TraceAnnotator::~TraceAnnotator() {

}

void TraceAnnotator::processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback) {
	vector<ExtendedPoint*> processed;
	int index = frame->index;
	// draw points of scene traces

	int size = 2;
	for(auto trace : video->pointTraces) {
		//if(i++ > 30) break;
		ExtendedPoint* point = trace->filter(index);
		if(point != NULL && point->trace != NULL) {
			processed.push_back(point);
			Point p = callback->getOutputImageCoordinates(point->coordinates);

			if(point->trace->type == PointTrace::scene) {
				circle(*image, p, size*2, trace->color);
			}
			else if(point->trace->type == PointTrace::object) {
				Point p1 = p, p2 = p;
				p1.x -= size; p1.y -= size;
				p2.x += size; p2.y += size;
				rectangle(*image, p1, p2, trace->color);
			}
			else {
				Point p1 = p, p2 = p;
				p1.y += size; p2.y -= size;
				line(*image, p1, p2, trace->color);
				p1 = p, p2 = p;
				p1.x += size; p2.x -= size;
				line(*image, p1, p2, trace->color);
			}

			ExtendedPoint* nextPoint = trace->filter(index+1);
			if(nextPoint != NULL) {
				if(nextPoint->coordinates != point->coordinates) {
					Point p2 = callback->getOutputImageCoordinates(nextPoint->coordinates);
					line(*image, p, p2, 1);
				}
			}
		}
	}

	// draw other points
	sort(frame->keypoints.begin(), frame->keypoints.end());
	sort(processed.begin(), processed.end());
	vector<ExtendedPoint*> otherPoints(frame->keypoints.size());
	auto end = set_difference(frame->keypoints.begin(), frame->keypoints.end(),
			processed.begin(), processed.end(), otherPoints.begin());
	cout << "other points " << (end-otherPoints.begin()) << endl;
	for(auto it = otherPoints.begin(); it!=end; it++) {
		Point2f p = callback->getOutputImageCoordinates((*it)->coordinates);
		Scalar color = Scalar(255,255,255);
		Point p1 = p, p2 = p;
		int diff = 1;
		p1.x -= diff; p1.y -= diff;
		p2.x += diff; p2.y += diff;
		line(*image, p1, p2, color);
		p1 = p, p2 = p;
		p1.x += diff; p1.y -= diff;
		p2.x -= diff; p2.y += diff;
		line(*image, p1, p2, color);
	}
}
