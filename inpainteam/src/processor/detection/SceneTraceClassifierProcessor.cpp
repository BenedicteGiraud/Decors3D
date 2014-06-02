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

		if(trace->points.size() >= 7) {
			//if(i > 30) break;
			double distance = 0;
			double threshold;
			ExtendedPoint* last = NULL;

			if(video->homographiesToLastFrame.size() != 0) {
				Mat homography = Mat::eye(3, 3, CV_64F);

				int homographiesIndex = 0;
				vector<Mat>::iterator it = video->homographiesToLastFrame.begin(), end = it;
				for(pair<Frame*, ExtendedPoint*> point : trace->points) {
					if(last != NULL) {
						for(; homographiesIndex < point.second->frame->index; it++, homographiesIndex++) {
							homography = Tools::concatenateHomography(homography,*it);
						}
						Point2f projectedPoint = Tools::applyHomography(homography,last->coordinates);
						double n = norm(projectedPoint - point.second->coordinates);
						distance += n*n;
					}
					last = point.second;
				}
				threshold = 1.5;
				cout << "distance A " << distance/ trace->points.size() << endl;
			}
			else {
				for(pair<Frame*, ExtendedPoint*> point : trace->points) {
					if(last != NULL) {
						double n = norm(last->coordinates - point.second->coordinates);
						distance += n*n;
					}
					last = point.second;
				}
				cout << "distance B " << distance/ trace->points.size() << endl;
				threshold = 0.10;
			}

			if(distance / trace->points.size() < threshold) {
				video->sceneTraces.push_back(trace);
			}
			else {
				video->objectTraces.push_back(trace);
			}

		}
	}
}

