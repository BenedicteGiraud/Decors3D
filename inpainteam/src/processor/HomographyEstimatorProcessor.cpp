/*
 * HomographyEstimatorProcessor.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include "HomographyEstimatorProcessor.h"

#include <cv.h>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/PointTrace.h"

using namespace cv;
using namespace std;

HomographyEstimatorProcessor::HomographyEstimatorProcessor() {

}

HomographyEstimatorProcessor::~HomographyEstimatorProcessor() {

}

void HomographyEstimatorProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {

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

	// TODO: use scene / object information
	if(points1.size() < 4 || points2.size() < 4) {
		cout << "No points for frame " << frame1->index << " and " << frame2->index << endl;
		Mat homography;
		if(video->homographies.size() > 0) {
			homography = video->homographies.back();
		}
		else {
			homography = (Mat_<double>(3,3)
					<< 1, 0, 0,
					0 , 1, 0);
		}
		video->homographies.push_back(homography);
	}
	else {
		cout << "Estimating homography with "
				<< points1.size() << " and " << points2.size() << " points"
				<< " in " << traces->size() << " traces " << endl;
		Mat homography = findHomography(points1, points2, CV_RANSAC, 1);
		video->homographies.push_back(homography);
	}

}
