#ifndef VIDEO_H
#define VIDEO_H

#include <vector>
#include "entities/Frame.h"
#include "VideoPlayer.h"
#include "entities/PointTrace.h"

#include "frame-processor/DoubleFrameProcessor.h"
#include "frame-processor/FrameProcessor.h"

class VideoPlayer;
class FrameProcessor;
class DoubleFrameProcessor;

class Video
{
private:
	int framesPerSecond;

public:
	std::vector<Frame> frames;
	std::vector<PointTrace> traces;

	Video();
	Video(std::string filename);
	void write(std::string filename);

	void play();
	VideoPlayer getPlayer();

	void applyFrameProcessor(FrameProcessor &processor);
	void applyDoubleFrameProcessor(DoubleFrameProcessor &processor);
};

#endif // VIDEO_H
