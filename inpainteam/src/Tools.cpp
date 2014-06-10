/*
 * Tools.cpp
 *
 *  Created on: 2014/05/31
 *      Author: tr
 */

#include "Tools.h"

#include <cv.h>
#include <highgui.h>

using namespace cv;

Tools::Tools() {
	// TODO Auto-generated constructor stub

}

Tools::~Tools() {
	// TODO Auto-generated destructor stub
}

Mat Tools::showNormalizedHistogram(string window, vector<float> data, int height) {
	namedWindow(window, WINDOW_NORMAL);

	if(height < 0) height = data.size();
	Mat histogram(height, data.size(), CV_8U);

	for(int i=0; i<data.size(); i++) {
		int lineHeight = height * data[i];
		line(histogram, Point(i,0), Point(i, lineHeight), Scalar(127,0,0));
	}

	imshow(window, histogram);

	return histogram;
}
