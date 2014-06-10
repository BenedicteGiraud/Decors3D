#ifndef VIDEO_H
#define VIDEO_H

#include <vector>
#include "entities/PointTrace.h"
#include "visualization/VideoPlayer.h"

class VideoPlayer;
class VideoProcessor;
class FrameProcessor;
class DoubleFrameProcessor;
class Frame;
class FrameTrace;

class Video
{
private:
	double framesPerSecond;

public:
	std::vector<Frame*> frames;
	std::vector<PointTrace*> pointTraces;

	std::vector<PointTrace*> sceneTraces;
	std::vector<PointTrace*> objectTraces;

	Video();
	Video(std::string filename);
	void write(std::string filename);
	void write(std::string filename, double framesPerSecond);
    Video& operator<<(const Mat& image);

	void play();
	VideoPlayer getPlayer();

	void applyVideoProcessor(VideoProcessor &processor);
	void applyFrameProcessor(FrameProcessor &processor);
	void applyDoubleFrameProcessor(DoubleFrameProcessor &processor);

	bool getHomography(Frame *from, Frame *to, Mat &homography);
};

#endif // VIDEO_H
