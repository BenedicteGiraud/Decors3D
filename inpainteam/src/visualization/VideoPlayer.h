/*
 * VideoPlayer.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef VIDEOPLAYER_H_
#define VIDEOPLAYER_H_

#include "processor/ProcessorCallback.h"

class Video;
class FrameProcessor;
class VideoProvider;
class VideoPlayer;

class VideoPlayerProcessorCallback : public ProcessorCallback {
	friend VideoPlayer;

	VideoPlayer *player;
	VideoPlayerProcessorCallback(VideoPlayer *player) : player(player) {}

	virtual void refreshGui();
};

class VideoPlayer {
private:
	int framesPerSecond;
	VideoProvider *provider;
	string outputDirectory;
	VideoPlayerProcessorCallback callback = VideoPlayerProcessorCallback(this);

public:
	VideoPlayer();
	virtual ~VideoPlayer();

	void setVideo(Video *video);
	void setVideoProvider(VideoProvider *provider);
	void setFramesPerSecond(int framesPerSecond);
	void setOutputDirectory(string outputDirectory);

	void play();
	void refresh();

	void mouseEvent(int event, int x, int y, int flags);
};

#endif /* VIDEOPLAYER_H_ */
