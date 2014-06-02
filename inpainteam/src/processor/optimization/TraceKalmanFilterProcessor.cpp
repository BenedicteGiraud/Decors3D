/*
 * TraceKalmanFilterProcessor.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include <cv.h>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/PointTrace.h"
#include "entities/ExtendedPoint.h"

#include "TraceKalmanFilterProcessor.h"

TraceKalmanFilterProcessor::TraceKalmanFilterProcessor() {

}

TraceKalmanFilterProcessor::~TraceKalmanFilterProcessor() {

}

void TraceKalmanFilterProcessor::processStart(Video* video, Frame* firstFrame) {
	for(PointTrace* trace : video->pointTraces) {
		if(trace == NULL) continue;
		ExtendedPoint* ep = trace->firstPoint();
		if(ep == NULL) continue;

		trace->kalmanFilter = KalmanFilter(4, 2, 0);
		trace->kalmanFilter.transitionMatrix = *(Mat_<float>(4, 4) << 1,0,0.1,0,   0,1,0,0.1,  0,0,1,0,  0,0,0,1);

		trace->kalmanFilter.statePre.at<float>(0) = ep->coordinates.x;
		trace->kalmanFilter.statePre.at<float>(1) = ep->coordinates.y;
		trace->kalmanFilter.statePre.at<float>(2) = 0;
		trace->kalmanFilter.statePre.at<float>(3) = 0;

		setIdentity(trace->kalmanFilter.measurementMatrix);
		setIdentity(trace->kalmanFilter.processNoiseCov, Scalar::all(1));
		setIdentity(trace->kalmanFilter.measurementNoiseCov, Scalar::all(1));
		setIdentity(trace->kalmanFilter.errorCovPost, Scalar::all(1));
	}
}

void TraceKalmanFilterProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {

	for(ExtendedPoint* ep1 : frame1->keypoints) {
		PointTrace* trace = ep1->trace;
		if(trace == NULL) continue;
		ExtendedPoint* ep2 = trace->filter(frame2);
		if(ep1 == NULL || ep2 == NULL) continue;

		Mat_<float> measurement(2,1); measurement.setTo(Scalar(0));

		// First predict, to update the internal statePre variable
		Mat prediction = trace->kalmanFilter.predict();
		Point predictPt(prediction.at<float>(0),prediction.at<float>(1));

		// Get mouse point
		measurement(0) = ep2->coordinates.x;
		measurement(1) = ep2->coordinates.y;

		Point measPt(measurement(0),measurement(1));

		// The "correct" phase that is going to use the predicted value and our measurement
		Mat estimated = trace->kalmanFilter.correct(measurement);
		Point statePt(estimated.at<float>(0),estimated.at<float>(1));
		ep2->coordinates = statePt;
	}
}
