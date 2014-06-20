#ifndef VIDEO_H
#define VIDEO_H

#include <vector>
#include "entities/PointTrace.h"
#include "visualization/VideoPlayer.h"
#include <map>

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
	std::map<std::pair<int, int>, Mat> getHomographyCache;

public:
	std::vector<Frame*> frames;
	std::vector<PointTrace*> pointTraces;

	std::map<PointTrace::Type, int> getPointTraceCount();
	std::vector<PointTrace*> filterPointTraces(PointTrace::Type type);

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
    void applyDoubleFrameProcessorInverse(DoubleFrameProcessor &processor);

	bool getHomography(Frame *from, Frame *to, Mat &homography);
};

#endif // VIDEO_H
