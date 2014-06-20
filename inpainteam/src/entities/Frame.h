/*
 * Frame.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <map>
#include <cv.h>

#include "entities/ExtendedPoint.h"

class Video;
class ExtendedPoint;

class Frame {
public:
	// Reference to Video and index of frame
	Video* video;
	int index;

	// content of frame
	cv::Mat image;

	// detected keypoints
	std::vector<ExtendedPoint*> keypoints;
	std::vector<cv::KeyPoint> rawKeypoints;
	cv::Mat rawDescriptors;

	// homography
	cv::Mat homographyToBeginning;
	cv::Mat homographyToLastFrame;

	// fundamental matrix
	cv::Mat fundamentalMatrixToLastFrame;

	Frame(cv::Mat image, Video* video, int index);
	virtual ~Frame();

	ExtendedPoint* getNearestKeyPoint(cv::Point2f point);
	std::multimap<double, ExtendedPoint*> getNearestKeyPoints(cv::Point2f point, double maxDistance);
};

#endif /* FRAME_H_ */
