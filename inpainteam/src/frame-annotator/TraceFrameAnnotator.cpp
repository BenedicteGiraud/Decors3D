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
using namespace std;

TraceFrameAnnotator::TraceFrameAnnotator() {

}

TraceFrameAnnotator::~TraceFrameAnnotator() {

}

vector<KeyPoint> filter(Frame* frame, vector<ExtendedPoint*> points) {
	vector<KeyPoint> result;
	for(auto point : points) {
		if(point->frame == frame) {
			result.push_back(point->keypoint);
		}
	}
	return result;
}

void TraceFrameAnnotator::annotate(Video* video, Frame* frame, cv::Mat* image) {
	Mat annotatedImage;
	int i = 0;
	cout << "scene " << video->sceneTraces.size() << " object " << video->objectTraces.size() << endl;
	for(auto trace : video->sceneTraces) {
		//if(i++ > 30) break;
		vector<KeyPoint> todraw = filter(frame, trace->points);
		drawKeypoints(*image, todraw, *image, trace->color, DrawMatchesFlags::DRAW_OVER_OUTIMG);
	}

	i=0;
	for(auto trace : video->objectTraces) {
		vector<KeyPoint> todraw = filter(frame, trace->points);
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