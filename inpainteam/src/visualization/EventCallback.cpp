/*
 * EventCallback.cpp
 *
 *  Created on: 2014/06/05
 *      Author: tr
 */

#include <cv.h>

#include "processor/FrameProcessor.h"
#include "visualization/VideoPlayer.h"

#include "EventCallback.h"

using namespace std;
using namespace cv;

EventCallback::EventCallback(VideoPlayer* player) {
	this->player = player;
}

EventCallback::~EventCallback() {

}

void EventCallback::eventCallback(int event, int x, int y, int flags) {
	// event: EVENT_LBUTTONDOWN, EVENT_RBUTTONDOWN, EVENT_MBUTTONDOWN, EVENT_MOUSEMOVE
	if  ( event == EVENT_LBUTTONDOWN )
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		Point2f point(x, y);
		cout << "original position: " << player->annotator->getInputImageCoordinates(point) << endl;
	}
}

void mouseCallbackFunction(int event, int x, int y, int flags, void* userdata) {
	((EventCallback*)userdata)->eventCallback(event, x, y, flags);
}

void EventCallback::setMouseCallback(string windowName) {
	cv::setMouseCallback(windowName, &mouseCallbackFunction, (void*)this);
}
