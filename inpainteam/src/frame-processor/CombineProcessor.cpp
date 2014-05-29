/*
 * CombineProcessor.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include "entities/Video.h"
#include "entities/Frame.h"

#include "CombineProcessor.h"

CombineProcessor::CombineProcessor() {
	// TODO Auto-generated constructor stub

}

CombineProcessor::~CombineProcessor() {
	// TODO Auto-generated destructor stub
}

void CombineProcessor::processStart(Video* video) {
	int i = -1;
	for(auto trace : video->pointTraces) {
		i++;

		if(trace->points.size() >= 4) {
			//if(i > 30) break;
			double distance = 0;
			ExtendedPoint* last = NULL;
			for(ExtendedPoint* point : trace->points) {
				if(last != NULL) {
					distance += norm(last->keypoint.pt - point->keypoint.pt);
				}
				last = point;
			}

			if(distance / trace->points.size() < 1) {
				video->sceneTraces.push_back(trace);
			}
			else {
				video->objectTraces.push_back(trace);
			}

		}
	}

}

void CombineProcessor::processFrame(Video* video, Frame* frame) {

}

void CombineProcessor::processEnd(Video* video) {

}
