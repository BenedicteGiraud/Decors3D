/*
 * KeyPointProcessor.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include "KeyPointProcessor.h"

KeyPointProcessor::KeyPointProcessor() {
	// TODO Auto-generated constructor stub

}

KeyPointProcessor::~KeyPointProcessor() {
	// TODO Auto-generated destructor stub
}

void KeyPointProcessor::processFrame(Video* video, Frame* frame) {
	if(frame->keypoints.size() == 0) {
		frame->detectKeyPoints();
	}
}
