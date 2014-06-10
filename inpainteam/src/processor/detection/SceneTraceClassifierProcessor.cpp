/*
 * SceneTraceClassifierProcessor.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include "entities/Video.h"
#include "entities/Frame.h"
#include "Tools.h"

#include "SceneTraceClassifierProcessor.h"

using namespace std;

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

		if(trace->points.size() >= 2) {
			//if(i > 30) break;
			double distance = 0;
			double threshold;
			ExtendedPoint* last = NULL;

			Mat homography = Mat::eye(3, 3, CV_32F);

			int homographiesIndex = 0;
			for(pair<int, ExtendedPoint*> point : trace->points) {
				if(last != NULL) {
					bool homographyIsValid = video->getHomography(last->frame, point.second->frame, homography);
					Point2f destPoint;
					if(homographyIsValid) {
						destPoint = Tools::applyHomography(homography,last->coordinates);
					}
					else {
						destPoint = last->coordinates;
					}
					double n = norm(destPoint - point.second->coordinates);
					distance += n*n;
				}
				last = point.second;
			}
			threshold = 1.5;
			cout << "distance A " << distance/ trace->points.size() << endl;

			if(distance / trace->points.size() < threshold) {
				trace->type = PointTrace::scene;
				video->sceneTraces.push_back(trace);
			}
			else {
				trace->type = PointTrace::object;
				video->objectTraces.push_back(trace);
			}

		}
	}
}

