/*
 * VideoProvider.h
 *
 *  Created on: 2014/06/05
 *      Author: tr
 */

#ifndef VIDEOPROVIDER_H_
#define VIDEOPROVIDER_H_

#include <cv.h>

using namespace cv;

class VideoProvider {
public:
	VideoProvider();
	virtual ~VideoProvider();

	virtual void seek(int index) = 0;
	virtual void seekRelative(int index) = 0;

	virtual void start() = 0;
	virtual Mat getImage() = 0;
	virtual void finish() = 0;

	virtual void mouseEventCallback(int event, int x, int y, int flags);
};

#endif /* VIDEOPROVIDER_H_ */
