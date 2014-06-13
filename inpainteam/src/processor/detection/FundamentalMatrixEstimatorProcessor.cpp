/*
 * FundamentalMatrixEstimatorProcessor.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include <cv.h>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/PointTrace.h"
#include "Tools.h"

#include "FundamentalMatrixEstimatorProcessor.h"

using namespace cv;
using namespace std;

FundamentalMatrixEstimatorProcessor::FundamentalMatrixEstimatorProcessor() {

}

FundamentalMatrixEstimatorProcessor::~FundamentalMatrixEstimatorProcessor() {

}

void FundamentalMatrixEstimatorProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
	vector<PointTrace*>* traces;

	if(video->sceneTraces.size() < 10) {
		traces = (&video->pointTraces);
	}
	else {
		traces = (&video->sceneTraces);
	}

	vector<Point2f> points1;
	vector<Point2f> points2;

	for(PointTrace* trace : *traces) {
		ExtendedPoint* ep1 = trace->filter(frame1);
		ExtendedPoint* ep2 = trace->filter(frame2);

		if(ep1 == NULL || ep2 == NULL) continue;

		points1.push_back(ep1->coordinates);
		points2.push_back(ep2->coordinates);
	}

	if(points1.size() < 8 || points2.size() < 8) return;

	vector<uchar> classification;
	Mat fundamentalMat = findFundamentalMat(points1, points2, CV_FM_RANSAC,
			20.0, 0.50, classification);

	frame2->fundamentalMatrixToLastFrame =  fundamentalMat;
	cout << "fundamental matrix type " << fundamentalMat.type() << endl;

	// Reclassify
	video->sceneTraces.clear();
	video->objectTraces.clear();
	for(int i=0; i<classification.size(); i++) {
		if(classification[i] == 1) {
			video->pointTraces[i]->type = PointTrace::scene;
			video->sceneTraces.push_back(video->pointTraces[i]);
		}
		else {
			video->pointTraces[i]->type = PointTrace::object;
			video->objectTraces.push_back(video->pointTraces[i]);
		}
	}
}
