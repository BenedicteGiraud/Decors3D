#ifndef VIDEO_H
#define VIDEO_H

#include <vector>
#include "entities/Frame.h"
#include "VideoPlayer.h"
#include "PointTrace.h"

class VideoPlayer;

class Video
{
private:
	int framesPerSecond;

public:
	std::vector<Frame> frames;
	std::vector<PointTrace> traces;

	Video();
	Video(std::string filename);

	void play();
	VideoPlayer getPlayer();

	void write(std::string filename);
};

#endif // VIDEO_H
