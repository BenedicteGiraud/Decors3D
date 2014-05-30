/*
 * SceneTraceClassifierProcessor.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include "entities/Video.h"
#include "entities/Frame.h"

#include "SceneTraceClassifierProcessor.h"

SceneTraceClassifierProcessor::SceneTraceClassifierProcessor() {

}

SceneTraceClassifierProcessor::~SceneTraceClassifierProcessor() {

}

void SceneTraceClassifierProcessor::process(Video* video) {

	video->sceneTraces.clear();
	video->objectTraces.clear();

	int i = -1;
	for(auto trace : video->pointTraces) {
		i++;

		if(trace->points.size() >= 4) {
			//if(i > 30) break;
			double distance = 0;
			ExtendedPoint* last = NULL;

			if(video->homographies.size() != 0) {
				Mat homography = Mat::eye(3, 3, CV_64F);

				int homographiesIndex = 0;
				vector<Mat>::iterator it = video->homographies.begin(), end = it;
				for(ExtendedPoint* point : trace->points) {
					if(last != NULL) {
						for(; homographiesIndex < point->frame->index; it++, homographiesIndex++) {
							homography = ExtendedPoint::concatenateHomography(homography,*it);
						}
						Point2f projectedPoint = ExtendedPoint::applyHomography(homography,last->keypoint.pt);
						distance += norm(projectedPoint - point->keypoint.pt);
					}
					last = point;
				}
			}
			else {
				for(ExtendedPoint* point : trace->points) {
					if(last != NULL) {
						double n = norm(last->keypoint.pt - point->keypoint.pt);
						distance += n * n;
					}
					last = point;
				}
			}

			if(distance / trace->points.size() < 0.7) {
				video->sceneTraces.push_back(trace);
			}
			else {
				video->objectTraces.push_back(trace);
			}

		}
	}
}

