#ifndef VIDEO_H
#define VIDEO_H

#include <vector>
#include "Frame.h"

class Video
{
private:
	std::vector<Frame> frames;

public:
	Video();
	Video(std::string filename);

	void play();
	void play(int framesPerSecond);
};

#endif // VIDEO_H
