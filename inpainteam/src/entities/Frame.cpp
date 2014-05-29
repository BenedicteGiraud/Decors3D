/*
 * Frame.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include "entities/Frame.h"

using namespace cv;
#include <cv.h>

Frame::Frame(Mat image) {
	this->image = image;
}

Frame::~Frame() {

}

void Frame::detectKeyPoints() {


}
