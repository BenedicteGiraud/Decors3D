/*
 * Frame.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef FRAME_H_
#define FRAME_H_

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

	void detectKeyPoints();
};

#endif /* FRAME_H_ */
