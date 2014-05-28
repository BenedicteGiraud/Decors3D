/*
 * Frame.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include "Frame.h"

using namespace cv;
#include <cv.h>
#include <highgui.h>

#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>

Frame::Frame(Mat image) {
	this->image = image;
}

Frame::~Frame() {

}

void Frame::detectKeyPoints() {

	SurfFeatureDetector detector(
			100, // hessianThreshold
			1, // nOctaves
			4, // nOctaveLayers
			true, // extended
			false // upright
			);

	/*GFTTDetector detector(
			10000, // maxCorners
			0.02, // qualityLevel
			4, // minDistance
			5, // blockSize
			true, // useHarrisDetector
			0.04); // k*/


	detector.detect(image, keypoints);
}
