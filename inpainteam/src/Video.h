#ifndef VIDEO_H
#define VIDEO_H

#include <vector>
#include "Frame.h"
#include "VideoPlayer.h"

class VideoPlayer;

class Video
{
private:
	int framesPerSecond;

public:
	std::vector<Frame> frames;

	Video();
	Video(std::string filename);

	void play();
	VideoPlayer getPlayer();

	void write(std::string filename);
};

#endif // VIDEO_H
