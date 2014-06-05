/*
 * EventCallback.h
 *
 *  Created on: 2014/06/05
 *      Author: tr
 */

#ifndef EVENTCALLBACK_H_
#define EVENTCALLBACK_H_

#include <cv.h>
#include <highgui.h>

class VideoPlayer;

using namespace cv;

class EventCallback {
public:
	VideoPlayer* player;

	EventCallback(VideoPlayer* player);
	virtual ~EventCallback();

	void eventCallback(int event, int x, int y, int flags);

	void setMouseCallback(string windowName);
};

#endif /* EVENTCALLBACK_H_ */
