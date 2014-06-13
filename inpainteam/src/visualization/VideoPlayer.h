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
class VideoProvider;

class VideoPlayer {
private:
	int framesPerSecond;
	VideoProvider *provider;
	string outputDirectory;

public:
	VideoPlayer();
	virtual ~VideoPlayer();

	void setVideo(Video *video);
	void setVideoProvider(VideoProvider *provider);
	void setFramesPerSecond(int framesPerSecond);
	void setOutputDirectory(string outputDirectory);

	void play();
};

#endif /* VIDEOPLAYER_H_ */
