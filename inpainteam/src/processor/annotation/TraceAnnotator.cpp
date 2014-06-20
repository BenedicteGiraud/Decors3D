/*
 * TraceAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>
#include <highgui.h>

#include "TraceAnnotator.h"
#include "entities/ExtendedPoint.h"
#include "entities/Video.h"
#include "entities/Frame.h"
#include "processor/detection/KeyPointProcessor.h"

using namespace cv;
using namespace std;

TraceAnnotator::TraceAnnotator() {
	this->video = NULL;
	this->frame = NULL;
}

TraceAnnotator::~TraceAnnotator() {

}

void TraceAnnotator::processStart(Video* video, Frame* firstframe) {
	this->video = video;
}

void TraceAnnotator::printSelectionInformation(Selection &s1, Selection &s2) {
	ExtendedPoint* point1 = s1.selectedPoint;
	ExtendedPoint* point2 = s2.selectedPoint;

	if(point1 == NULL && point2 == NULL) return;

	if(point1 != NULL) {
		cout << "P1: (" << point1->coordinates.x << "," << point1->coordinates.y << ") ";
	}
	if(point2 != NULL) {
		cout << "P2: (" << point2->coordinates.x << "," << point2->coordinates.y << ") ";
	}

	if(point1 == NULL || point2 == NULL) {
		cout << endl;
		return;
	}

	double distance = norm(point1->coordinates - point2->coordinates);
	cout << "distance " << distance << " ";
	double difference = KeyPointProcessor::descriptorDistance(point1->descriptor, point2->descriptor);
	Mat diff = abs(point1->descriptor - point2->descriptor);
	KeyPointProcessor::visualizeDescriptor("descDiff", diff);
	cout << "descriptor diff " << difference << " ";
	cout << endl;
}

void TraceAnnotator::processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback) {
	this->frame = frame;
	printSelectionInformation(selection1, selection2);
	vector<ExtendedPoint*> processed;
	int index = frame->index;
	// draw points of scene traces

	int size = 4;
	for(auto trace : video->pointTraces) {
		//if(i++ > 30) break;
		ExtendedPoint* point = trace->filter(index);
		if(point != NULL && point->trace != NULL) {
			processed.push_back(point);
			Point p = callback->getOutputImageCoordinates(point->coordinates);

			int thickness = 1;
			if(selection1.isSelected(point) || selection2.isSelected(point)) {
				thickness = 2;
			}

			if(point->trace->type == PointTrace::scene) {
				circle(*image, p, size*2, trace->color, thickness);
			}
			else if(point->trace->type == PointTrace::object) {
				Point p1 = p, p2 = p;
				p1.x -= size; p1.y -= size;
				p2.x += size; p2.y += size;
				rectangle(*image, p1, p2, trace->color, thickness);
			}
			else {
				Point p1 = p, p2 = p;
				p1.y += size; p2.y -= size;
				line(*image, p1, p2, trace->color, thickness);
				p1 = p, p2 = p;
				p1.x += size; p2.x -= size;
				line(*image, p1, p2, trace->color, thickness);
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

	for(auto it = otherPoints.begin(); it!=end; it++) {
		int thickness = 1;
		if(selection1.isSelected(*it) || selection2.isSelected(*it)) {
			thickness = 2;
		}

		Point2f p = callback->getOutputImageCoordinates((*it)->coordinates);
		Scalar color = Scalar(255,255,255);
		Point p1 = p, p2 = p;
		int diff = 1;
		p1.x -= diff; p1.y -= diff;
		p2.x += diff; p2.y += diff;
		line(*image, p1, p2, color, thickness);
		p1 = p, p2 = p;
		p1.x += diff; p1.y -= diff;
		p2.x -= diff; p2.y += diff;
		line(*image, p1, p2, color, thickness);
	}
}

void TraceAnnotator::mouseEventCallback(int event, int x, int y, int flags, ProcessorCallback* callback) {
	if(frame == NULL) return;
	if (event == EVENT_LBUTTONDOWN) {
        point = callback->getInputImageCoordinates(Point2f(x,y));
        ExtendedPoint *ep = //frame->getNearestKeyPoint(point);
        // TODO: memory leak
        new ExtendedPoint(point, NULL);
        if(ep != NULL) {
            Mat desc;
            KeyPointProcessor::extractPatchDescriptor(frame->image, desc, point);
            ep->descriptor = desc;
            selection1 = Selection(ep);
			callback->refreshGui();
			KeyPointProcessor::visualizeDescriptor("desc1", desc);
		}
	}
	else if(event == EVENT_RBUTTONDOWN) {
		Point2f point = callback->getInputImageCoordinates(Point2f(x,y));
        ExtendedPoint *ep = //frame->getNearestKeyPoint(point);
        // TODO: memory leak
        new ExtendedPoint(point, NULL);
        if(ep != NULL) {
            Mat desc;
            KeyPointProcessor::extractPatchDescriptor(frame->image, desc, point);
            ep->descriptor = desc;
            selection2 = Selection(ep);
			callback->refreshGui();
			KeyPointProcessor::visualizeDescriptor("desc2", desc);
		}
	}
}
