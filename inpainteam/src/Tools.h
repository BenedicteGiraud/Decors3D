/*
 * Tools.h
 *
 *  Created on: 2014/05/31
 *      Author: tr
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <cv.h>

using namespace cv;

class Tools {
public:
	Tools();
	virtual ~Tools();

	inline static void verticalConcatenateMatrices(cv::Mat& input, cv::Mat& add, cv::Mat& output) {
		if(input.rows == 0) {
			output = add;
		}
		else if(add.rows == 0) {
			output = input;
		}
		else {
			vconcat(input, add, output);
		}
	}

	inline static cv::Point2f applyHomography(cv::Mat homography, cv::Point2f point) {

		Mat p(3, 1, CV_64F);
		p.at<double>(0,0) = point.x;
		p.at<double>(1,0) = point.y;
		p.at<double>(2,0) = 1;

		Mat projected = homography * p;

		Point2f result;
		result.x = projected.at<double>(0,0) / projected.at<double>(2,0);
		result.y = projected.at<double>(1,0) / projected.at<double>(2,0);

		return result;
	}

	inline static cv::Mat concatenateHomography(cv::Mat homography1, cv::Mat homography2) {
		Mat h1 = homography1;
		Mat h2 = homography2;

		if(h1.rows != 3) {
			h1 = Mat::eye(3,3,CV_64F);
			h1.row(0) = homography1.row(0);
			h1.row(1) = homography1.row(1);
		}

		if(h2.rows != 3) {
			h2 = Mat::eye(3,3,CV_64F);
			h2.row(0) = homography2.row(0);
			h2.row(1) = homography2.row(1);
		}

		return h1 * h2;
	}

	inline static cv::Mat getIdentityHomography() {
		return (Mat_<double>(3,3)
				<< 1, 0, 0,
				0, 1, 0,
				0, 0, 1);
	}
};

#endif /* TOOLS_H_ */
