/*
 * HomographyAnnotator.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cv.h>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/ExtendedPoint.h"

#include "HomographyAnnotator.h"

using namespace cv;
using namespace std;

HomographyAnnotator::HomographyAnnotator() {

}

HomographyAnnotator::~HomographyAnnotator() {

}

void HomographyAnnotator::processStart(Video* video) {
	this->points = vector<Point2f>();
	int x1 = video->frames.front()->image.cols / 3, x2 = 2*x1;
	int y1 = video->frames.front()->image.rows / 3, y2 = 2*y1;
	this->points.push_back(Point2f(x1, y1));
	this->points.push_back(Point2f(x2, y1));
	this->points.push_back(Point2f(x2, y2));
	this->points.push_back(Point2f(x1, y2));
}

void HomographyAnnotator::processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback) {
	vector<Point2f> transformedPoints = this->points;
	if(frame->index-1 < video->homographiesToBeginning.size()) {
		perspectiveTransform(this->points, transformedPoints, video->homographiesToBeginning.at(frame->index-1));
	}

	Point2f last; bool first = true;
	for(auto point : transformedPoints) {
		cout << " " << point;
		if(!first) {
			line(*image, point, last, Scalar(0,0,0));
		}
		last = point;
		first = false;
	}
	cout << endl;
	line(*image, last, transformedPoints.front(), Scalar(0,0,0));
}
