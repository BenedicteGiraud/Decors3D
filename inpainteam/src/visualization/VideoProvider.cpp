/*
 * VideoProvider.cpp
 *
 *  Created on: 2014/06/05
 *      Author: tr
 */

#include <highgui.h>

#include "VideoProvider.h"

using namespace std;
using namespace cv;

VideoProvider::VideoProvider() {

}

VideoProvider::~VideoProvider() {

}

void VideoProvider::mouseEventCallback(int event, int x, int y, int flags) {
	if  ( event == EVENT_LBUTTONDOWN )
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		//Point2f point(x, y);
		//cout << "original position: " << provider->getInputImageCoordinates(point) << endl;
	}
}
