#ifndef VIDEO_H
#define VIDEO_H

#include <vector>
#include "Frame.h"

class Video
{
private:
	std::vector<Frame> frames;
	int framesPerSecond;

public:
	Video();
	Video(std::string filename);

	void play();
	void play(int framesPerSecond);

	void write(std::string filename);
};

#endif // VIDEO_H
