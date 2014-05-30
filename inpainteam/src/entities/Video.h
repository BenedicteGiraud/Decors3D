#ifndef VIDEO_H
#define VIDEO_H

#include <vector>
#include "entities/PointTrace.h"
#include "VideoPlayer.h"

class VideoPlayer;
class VideoProcessor;
class FrameProcessor;
class DoubleFrameProcessor;
class Frame;
class FrameTrace;

class Video
{
private:
	int framesPerSecond;

public:
	std::vector<Frame*> frames;
	std::vector<PointTrace*> pointTraces;

	std::vector<PointTrace*> sceneTraces;
	std::vector<PointTrace*> objectTraces;

	std::vector<cv::Mat> homographiesToLastFrame;
	std::vector<cv::Mat> homographiesToBeginning;

	Video();
	Video(std::string filename);
	void write(std::string filename);

	void play();
	VideoPlayer getPlayer();

	void applyVideoProcessor(VideoProcessor &processor);
	void applyFrameProcessor(FrameProcessor &processor);
	void applyDoubleFrameProcessor(DoubleFrameProcessor &processor);
};

#endif // VIDEO_H
