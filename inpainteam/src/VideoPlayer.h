/*
 * VideoPlayer.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef VIDEOPLAYER_H_
#define VIDEOPLAYER_H_

#include "FrameAnnotator.h"
#include "Video.h"

class Video;
class FrameAnnotator;

class VideoPlayer {
private:
	int framesPerSecond;
	FrameAnnotator *annotator;
	Video *video;

public:
	VideoPlayer();
	virtual ~VideoPlayer();

	void setVideo(Video *video);
	void setFramesPerSecond(int framesPerSecond);
	void setFramesAnnotator(FrameAnnotator *annotator);

	void play();
};

#endif /* VIDEOPLAYER_H_ */
