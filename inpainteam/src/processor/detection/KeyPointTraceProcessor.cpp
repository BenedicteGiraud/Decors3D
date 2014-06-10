/*
 * KeyPointTraceProcessor.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include <cv.h>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/PointTrace.h"
#include "entities/ExtendedPoint.h"


#include "KeyPointTraceProcessor.h"

using namespace cv;

KeyPointTraceProcessor::KeyPointTraceProcessor() {

}

KeyPointTraceProcessor::~KeyPointTraceProcessor() {

}

void KeyPointTraceProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
	int searchDistance = 10;
	// calculate matches between frame

	if(frame1->rawDescriptors.rows == 0 || frame2->rawDescriptors.rows == 0) return;

	BFMatcher matcher; //(NORM_HAMMING);
	vector<vector<DMatch>> matches;
	matcher.radiusMatch(frame1->rawDescriptors, frame2->rawDescriptors, matches, searchDistance, Mat(), true);

	for(vector<DMatch> matchList : matches) {
		if(matchList.size() == 0) continue;
		DMatch match = matchList.front();

		ExtendedPoint* ep1 = frame1->keypoints.at(match.queryIdx);
		ExtendedPoint* ep2 = frame2->keypoints.at(match.trainIdx);

		Point2f p1 = ep1->keypoint.pt;
		Point2f p2 = ep2->keypoint.pt;

		double distance = norm(p1 - p2);
		if(distance < searchDistance) {
			PointTrace* trace = ep1->getOrCreate();
			trace->addOrReplacePoint(ep2);
			ep2->trace = trace;
		}
	}

	// assign remaining points to traces
	vector<ExtendedPoint*> workingList(frame2->keypoints);
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
