/*
 * Frame.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include "Frame.h"

using namespace cv;

Frame::Frame(Mat image) {
	this->image = image;
}

Frame::~Frame() {

}

