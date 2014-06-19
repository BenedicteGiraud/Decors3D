/*
 * TraceAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef TraceAnnotator_H_
#define TraceAnnotator_H_

#include "entities/PointTrace.h"
#include "entities/ExtendedPoint.h"
#include "processor/FrameProcessor.h"
#include "visualization/EventListener.h"

struct Selection {
	Selection(ExtendedPoint* point) : selectedPoint(point) {}

	ExtendedPoint *selectedPoint;

	bool isSelected(ExtendedPoint *point) {
		if(selectedPoint==NULL || point == NULL) return false;
		return selectedPoint == point || (point->trace != NULL && point->trace == selectedPoint ->trace);
	}

	ExtendedPoint *getCorrespondingPoint(Frame *frame) {
		if(selectedPoint == NULL) return NULL;
		if(selectedPoint->frame == frame) return selectedPoint;
		return selectedPoint->trace->filter(frame);
	}
};

class TraceAnnotator : public FrameProcessor, public EventListener {
private:
	Video* video;
	Frame* frame;
	Selection selection1 = Selection(NULL);
	Selection selection2 = Selection(NULL);
    Point2f point;

public:
	TraceAnnotator();
	virtual ~TraceAnnotator();

	virtual void processStart(Video* video, Frame* firstframe);
	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback);

	virtual void mouseEventCallback(int event, int x, int y, int flags, ProcessorCallback* callback);

	void printSelectionInformation(Selection &s1, Selection &s2);
};

#endif /* TraceAnnotator_H_ */
