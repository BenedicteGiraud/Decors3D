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

void HomographyAnnotator::processFrame(Video* video, Frame* frame, cv::Mat* image) {
	Mat annotatedImage;
	int i = 0;
	if(frame->index == 0 || this->points.size() == 0) {
		this->points = vector<Point2f>();
		int x1 = image->cols / 3, x2 = 2*x1;
		int y1 = image->rows / 3, y2 = 2*y1;
		this->points.push_back(Point2f(x1, y1));
		this->points.push_back(Point2f(x2, y1));
		this->points.push_back(Point2f(x2, y2));
		this->points.push_back(Point2f(x1, y2));
	}
	else {
		if(frame->index-1 < video->homographies.size()) {
			perspectiveTransform(this->points, this->points, video->homographies.at(frame->index-1));
		}
	}

	i=0;
	Point2f last; bool first = true;
	for(auto point : this->points) {
		if(!first) {
			line(*image, point, last, Scalar(0,0,0));
		}
		last = point;
		first = false;
	}
	line(*image, last, this->points.front(), Scalar(0,0,0));
}
