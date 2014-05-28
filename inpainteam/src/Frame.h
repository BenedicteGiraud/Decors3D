/*
 * Frame.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <cv.h>

class Frame {
public:
	cv::Mat image;
	std::vector<cv::KeyPoint> keypoints;

	Frame(cv::Mat image);
	virtual ~Frame();

	void detectKeyPoints();
};

#endif /* FRAME_H_ */
