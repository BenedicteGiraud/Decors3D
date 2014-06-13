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


#include "KeyPointTraceProcessor.h"

using namespace cv;
using namespace std;

KeyPointTraceProcessor::KeyPointTraceProcessor() {

}

KeyPointTraceProcessor::~KeyPointTraceProcessor() {

}

bool checkAndAddToTrace(ExtendedPoint *ep1, ExtendedPoint *ep2) {
	int searchDistance = 10;
	Point2f p1 = ep1->keypoint.pt;
	Point2f p2 = ep2->keypoint.pt;

	double distance = norm(p1 - p2);
	if(distance < searchDistance) {
		PointTrace* trace = ep1->getOrCreate();
		if(trace->filter(ep2->frame->index) != NULL) return false;
		trace->addOrReplacePoint(ep2);
		ep2->trace = trace;
		return true;
	}
	return false;
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

void matchPoints(Frame* frame1, Frame* frame2) {
	cout << endl << " matching points " << endl;
	int searchDistance = min(frame1->image.rows, frame2->image.rows) / 5;
	double maxDescriptorDifference = .55;

	vector<ExtendedPoint*> points1 = frame1->keypoints;
	vector<pair<Point2f, ExtendedPoint*>> points2;
	Mat homography; // = frame2->homographyToLastFrame;

	typedef pair<ExtendedPoint*, ExtendedPoint*> ExtendedPointTuple;
	typedef pair<double, ExtendedPointTuple> QueueType;
	priority_queue<QueueType, vector<QueueType>, greater<QueueType>> candidates;

	for(ExtendedPoint* point : frame2->keypoints) {
		Point2f coordinates = Tools::applyHomography(homography, point->coordinates);
		points2.push_back(pair<Point2f, ExtendedPoint*>(coordinates, point));
	}

	double maxCombinedDistance = 0;
	for(ExtendedPoint *point1 : points1) {
		for(auto point2 : points2) {
			// compare descriptor
			double distance = norm(point1->coordinates - point2.first);
			if(distance > searchDistance) continue;
			double descriptorDifference = norm(point1->descriptor - point2.second->descriptor);
			if(descriptorDifference > maxDescriptorDifference) continue;
			//cout << "(" << point1 << "," << point2.second << endl;
			double combinedDistance = (2*(double)distance/searchDistance)+(descriptorDifference);
			if(maxCombinedDistance < combinedDistance) maxCombinedDistance = combinedDistance;

			candidates.push(QueueType(combinedDistance, ExtendedPointTuple(point1, point2.second)));
		}
	}

	cout << "queue" << endl;
	int count = 0;
	double quantilDistance = DBL_MAX;
	while(!candidates.empty()) {
		QueueType element = candidates.top();

		if(element.second.second->trace == NULL) {
			//cout << "adding " << element.second.first << "," << element.second.second << endl;
			checkAndAddToTrace(element.second.first, element.second.second);
		}
		count++;
		if(count > 0.5*candidates.size()) {
			if(quantilDistance > maxCombinedDistance) {
				quantilDistance = element.first;
			}
			if(element.first > quantilDistance+0.25*(maxCombinedDistance-quantilDistance)) break;
		}

		//cout << "pq: " << element.first << endl;
		candidates.pop();
	}


}

void assignRemainingPoints(Video* video, Frame* frame) {
	int searchDistance = 10;
	// assign remaining points to traces
	vector<ExtendedPoint*> workingList(frame->keypoints);
	for(auto it = workingList.begin(); it != workingList.end(); it++) {
		if((*it)->trace != NULL) continue;
		bool found = false;
		int minDistance = INT_MAX;
		PointTrace *bestTrace = NULL;
		for(auto trace = video->pointTraces.begin(); trace != video->pointTraces.end(); trace++) {
			// search for corresponding trace
			if((*trace)->points.size() < 1) continue;
			ExtendedPoint* last = (*trace)->lastPoint();

			double distance = norm(last->keypoint.pt - (*it)->keypoint.pt);
			if(distance < minDistance) {
				minDistance = distance;
				bestTrace = (*trace);
			}
		}

		if(minDistance < searchDistance) {
			bestTrace->addOrReplacePoint(*it);
			(*it)->trace = bestTrace;
			found = true;
			break;
		}

		if(!found) {
			// create new trace
			//(*it)->getOrCreate();
		}
	}
}

void KeyPointTraceProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
	int searchDistance = 10;
	// calculate matches between frame

	if(frame1->rawDescriptors.rows == 0 || frame2->rawDescriptors.rows == 0) return;

	matchPoints(frame1, frame2);

	assignRemainingPoints(video, frame2);
}
