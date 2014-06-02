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

using namespace cv;
using namespace std;

TraceAnnotator::TraceAnnotator() {

}

TraceAnnotator::~TraceAnnotator() {

}

void TraceAnnotator::processFrame(Video* video, Frame* frame, cv::Mat* image) {
	Mat annotatedImage;
	int i = 0;
	cout << "scene " << video->sceneTraces.size() << " object " << video->objectTraces.size() << endl;
	for(auto trace : video->sceneTraces) {
		//if(i++ > 30) break;
		ExtendedPoint* point = trace->filter(frame);
		if(point != NULL) {
			Point_<int> p(point->coordinates.x, point->coordinates.y);
			circle(*image, p, 4, trace->color);
		}
	}

	i=0;
	for(auto trace : video->objectTraces) {
		ExtendedPoint* point = trace->filter(frame);
		if(point != NULL) {
			Point p1 = point->coordinates, p2 = point->coordinates;
			int diff = 2;
			p1.x -= diff; p1.y -= diff;
			p2.x += diff; p2.y += diff;
			rectangle(*image, p1, p2, trace->color);
		}
	}
}
