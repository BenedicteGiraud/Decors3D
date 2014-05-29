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

void Frame::detectKeyPoints() {


}
