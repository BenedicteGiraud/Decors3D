/*
 * VideoPlayer.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef VIDEOPLAYER_H_
#define VIDEOPLAYER_H_

class Video;
class FrameProcessor;

class VideoPlayer {
private:
	int framesPerSecond;
	FrameProcessor *annotator;
	Video *video;

public:
	VideoPlayer();
	virtual ~VideoPlayer();

	void setVideo(Video *video);
	void setFramesPerSecond(int framesPerSecond);
	void setFramesAnnotator(FrameProcessor *annotator);

	void play();
};

#endif /* VIDEOPLAYER_H_ */
