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
	cout << "scene " << video->sceneTraces.size() << " object " << video->objectTraces.size() << endl;
	// draw points of scene traces
	for(auto trace : video->sceneTraces) {
		//if(i++ > 30) break;
		ExtendedPoint* point = trace->filter(index);
		if(point != NULL) {
			processed.push_back(point);
			Point p = callback->getOutputImageCoordinates(point->coordinates);
			circle(*image, p, 4, trace->color);

			ExtendedPoint* nextPoint = trace->filter(index+1);
			if(nextPoint != NULL) {
				if(nextPoint->coordinates != point->coordinates) {
					Point p2 = callback->getOutputImageCoordinates(nextPoint->coordinates);
					line(*image, p, p2, 1);
				}
			}
		}
	}

	// draw points of object traces
	for(auto trace : video->objectTraces) {
		ExtendedPoint* point = trace->filter(index);
		if(point != NULL) {
			processed.push_back(point);
			Point2f p = callback->getOutputImageCoordinates(point->coordinates);
			Point p1 = p, p2 = p;
			int diff = 2;
			p1.x -= diff; p1.y -= diff;
			p2.x += diff; p2.y += diff;
			rectangle(*image, p1, p2, trace->color);

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
