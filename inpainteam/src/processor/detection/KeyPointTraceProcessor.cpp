/*
 * KeyPointTraceProcessor.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include <cv.h>
#include <queue>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/PointTrace.h"
#include "entities/ExtendedPoint.h"

#include "Tools.h"

#include "KeyPointProcessor.h"
#include "KeyPointTraceProcessor.h"

using namespace cv;
using namespace std;

KeyPointTraceProcessor::KeyPointTraceProcessor() {

}

KeyPointTraceProcessor::~KeyPointTraceProcessor() {

}

inline double getCombinedDistance(double normedSpatialDistance, Mat desc1, Mat desc2) {
	double descriptorDifference = KeyPointProcessor::descriptorDistance(desc1, desc2);
	//cout << "(" << point1 << "," << point2.second << endl;
	return (20*(double)normedSpatialDistance)+(descriptorDifference);
}

bool checkAndAddToTrace(ExtendedPoint *ep1, ExtendedPoint *ep2) {
	Point2f p1 = ep1->keypoint.pt;
	Point2f p2 = ep2->keypoint.pt;

	PointTrace* trace = ep1->getOrCreate();
	if(trace->filter(ep2->frame->index) != NULL) return false;
	trace->addOrReplacePoint(ep2);
	ep2->trace = trace;
	return true;
}

void matchPointsBF(Frame*  frame1, Frame* frame2) {
	int searchDistance = 10;
	BFMatcher matcher; //(NORM_HAMMING);
	vector<vector<DMatch>> matches;
	matcher.radiusMatch(frame1->rawDescriptors, frame2->rawDescriptors, matches, searchDistance, Mat(), true);

	for(vector<DMatch> matchList : matches) {
		if(matchList.size() == 0) continue;
		DMatch match = matchList.front();

		ExtendedPoint* ep1 = frame1->keypoints.at(match.queryIdx);
		ExtendedPoint* ep2 = frame2->keypoints.at(match.trainIdx);
		checkAndAddToTrace(ep1, ep2);
	}
}

double matchPoints(Video* video, Frame* frame1, Frame* frame2) {
	cout << endl << " matching points " << endl;
	int searchDistance = min(frame1->image.rows, frame2->image.rows) / 5;
	double maxDescriptorDifference = DBL_MAX; //.55;
	bool createNewPoints = false;

	vector<ExtendedPoint*> points1 = frame1->keypoints;
	vector<pair<Point2f, ExtendedPoint*>> points2;
	Mat homography;
	video->getHomography(frame1, frame2, homography);

	typedef pair<ExtendedPoint*, ExtendedPoint*> ExtendedPointTuple;
	typedef pair<double, ExtendedPointTuple> QueueType;
	priority_queue<QueueType, vector<QueueType>, greater<QueueType>> candidates;

	for(ExtendedPoint* point : frame2->keypoints) {
		Point2f coordinates = Tools::applyHomography(homography, point->coordinates);
		points2.push_back(pair<Point2f, ExtendedPoint*>(coordinates, point));
		createNewPoints = true;
	}

	double maxCombinedDistance = 0;
	for(ExtendedPoint *point1 : points1) {
		for(auto point2 : points2) {
			// compare descriptor
			double spatialDistance = norm(point1->coordinates - point2.first);
			if(spatialDistance > searchDistance) continue;
			double combinedDistance = getCombinedDistance(spatialDistance/searchDistance,
					point1->descriptor, point2.second->descriptor);

			if(combinedDistance > searchDistance*maxDescriptorDifference) continue;

			candidates.push(QueueType(combinedDistance, ExtendedPointTuple(point1, point2.second)));
			if(maxCombinedDistance < combinedDistance) maxCombinedDistance = combinedDistance;
		}
	}

	cout << "queue" << endl;
	int countProcessed = 0;
	int countAdded = 0;
	double quantilDistance = DBL_MAX;
	double sumDescriptorDistance = 0;
	while(!candidates.empty()) {
		QueueType element = candidates.top();

		if(element.second.second->trace == NULL) {
			//cout << "adding " << element.second.first << "," << element.second.second << endl;
			if(checkAndAddToTrace(element.second.first, element.second.second)) {
				countAdded++;
				Mat desc1 = element.second.first->descriptor;
				Mat desc2 = element.second.second->descriptor;
				double distance = KeyPointProcessor::descriptorDistance(desc1, desc2);
				if(distance > sumDescriptorDistance) {
					sumDescriptorDistance = distance;
				}
			}
		}
		countProcessed++;
		if(countProcessed > 0.20*candidates.size()) {
			if(quantilDistance > maxCombinedDistance) {
				quantilDistance = element.first;
			}
			double threshold = quantilDistance+0.05*(maxCombinedDistance-quantilDistance);
			if(element.first > threshold) {
				cout << "KeyPointTraceProcessor: effective theshold" << threshold << endl;
				return threshold;
			}
		}
		candidates.pop();
	}
	return sumDescriptorDistance / countAdded;
}

void continueUnmatchedTraces(Video* video, Frame* frame1, Frame* frame2, double maxDescriptorDistance) {
	Mat homography;
	if(!video->getHomography(frame1, frame2, homography)) {
		return;
	}

	// TODO: add optical flow algorithm to adapt projected points
	for(ExtendedPoint *point : frame1->keypoints) {
		if(point->trace == NULL) continue;
		if(point->trace != NULL && point->trace->filter(frame2) != NULL) continue;

		Point2f projPoint = Tools::applyHomography(homography, point->coordinates);
		Mat desc;
		KeyPointProcessor::extractPatchDescriptor(frame2->image, desc, projPoint);

		double distance = getCombinedDistance(0, point->descriptor, desc);
		if(distance < maxDescriptorDistance) {
			ExtendedPoint* ep = KeyPointProcessor::addKeypoint(frame2, projPoint, desc, ExtendedPoint::projected);
			if(ep != NULL) {
				point->trace->addOrReplacePoint(ep);
			}
		}
	}
}

void KeyPointTraceProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
	int searchDistance = 10;
	// calculate matches between frame

	if(frame1->rawDescriptors.rows == 0 || frame2->rawDescriptors.rows == 0) return;

	double maxDescriptorDistance = matchPoints(video, frame1, frame2);
	continueUnmatchedTraces(video, frame1, frame2, maxDescriptorDistance);
}
