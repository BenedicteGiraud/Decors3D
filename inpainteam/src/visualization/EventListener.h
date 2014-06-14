/*
 * EventListener.h
 *
 *  Created on: 2014/06/14
 *      Author: tr
 */

#ifndef EVENTLISTENER_H_
#define EVENTLISTENER_H_

#include "processor/ProcessorCallback.h"

class EventListener {
public:
	EventListener();
	virtual ~EventListener();

	virtual void mouseEventCallback(int event, int x, int y, int flags, ProcessorCallback *callback) = 0;
};

#endif /* EVENTLISTENER_H_ */
