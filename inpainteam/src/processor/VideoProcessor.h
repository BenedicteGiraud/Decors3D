/*
 * VideoProcessor.h
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#ifndef VIDEOPROCESSOR_H_
#define VIDEOPROCESSOR_H_

class Video;

class VideoProcessor {
public:
	VideoProcessor();
	virtual ~VideoProcessor();

	virtual void process(Video* video) = 0;
};

#endif /* VIDEOPROCESSOR_H_ */
