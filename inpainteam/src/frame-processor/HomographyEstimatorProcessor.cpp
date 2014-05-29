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
	// TODO Auto-generated constructor stub

}

HomographyEstimatorProcessor::~HomographyEstimatorProcessor() {
	// TODO Auto-generated destructor stub
}

void HomographyEstimatorProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {

	vector<PointTrace*>* traces;

	if(video->sceneTraces.size() == 0) {
		traces = (&video->pointTraces);
	}
	else {
		traces = (&video->sceneTraces);
	}

	vector<Point2f> points1;
	vector<Point2f> points2;

	for(PointTrace* trace : *traces) {
		KeyPoint* keypoint1 = trace->filter(frame1);
		KeyPoint* keypoint2 = trace->filter(frame2);

		if(keypoint1 == NULL || keypoint2 == NULL) continue;

		points1.push_back(keypoint1->pt);
		points2.push_back(keypoint2->pt);
	}

	// TODO: use scene / object information
	if(points1.size() == 0 || points2.size() == 0) {
		cout << "No points for frame " << frame1->index << " and " << frame2->index << endl;
		Mat homography = video->homographies.back();
		video->homographies.push_back(homography);
	}
	else {
		cout << "Estimating homography with "
				<< points1.size() << " and " << points2.size() << " points"
				<< " in " << traces->size() << " traces " << endl;
		Mat homography = findHomography(points1, points2, CV_LMEDS, 1);
		video->homographies.push_back(homography);
	}

}
