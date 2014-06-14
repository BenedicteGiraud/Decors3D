/*
 * Frame.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include "entities/Frame.h"

using namespace cv;
#include <cv.h>

Frame::Frame(Mat image, Video* video, int index) {
	this->image = image;
	this->video = video;
	this->index = index;
}

Frame::~Frame() {

}

ExtendedPoint* Frame::getNearestKeyPoint(Point2f point) {
	double minDistance = DBL_MAX; ExtendedPoint *minPoint = NULL;
	for(ExtendedPoint* keypoint : keypoints) {
		double distance = norm(keypoint->coordinates - point);
		if(distance < minDistance) {
			minDistance = distance;
			minPoint = keypoint;
		}
	}
	return minPoint;
}
