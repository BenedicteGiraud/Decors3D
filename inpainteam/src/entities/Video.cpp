
#include <highgui.h>

#include "frame-processor/DoubleFrameProcessor.h"
#include "frame-processor/FrameProcessor.h"

#include "entities/Video.h"
#include "entities/Frame.h"


using namespace std;
using namespace cv;

Video::Video()
{
	framesPerSecond = 24;
}

Video::Video(string filename)
{
	VideoCapture inputVideo(filename);
	if(!inputVideo.isOpened()) {
		cout << "Video file " + filename + " couldn't be opened" << endl;
		throw -1;
	}
	framesPerSecond = inputVideo.get(CV_CAP_PROP_FPS);
	Mat image;
	int index = 0;
	while(true) {
		inputVideo >> image;
		if(image.empty()) break;

		frames.push_back(new Frame(image.clone(), this, index++));
	}
}

/**
 * Saves the video to a file using the XVID codec.
 * @param filename
 */
void Video::write(string filename) {
	int outputCodec = CV_FOURCC('X', 'V', 'I', 'D');

	VideoWriter outputVideo;
	int width = (int) (*frames.begin())->image.size().width;
	int height = (int) (*frames.begin())->image.size().height;
	Size size = Size(width, height);
	outputVideo.open(filename, outputCodec,
			framesPerSecond, size, true);

	for(auto it = frames.begin(); it != frames.end(); it++) {
		outputVideo << (*it)->image;
	}
}

/**
 *Plays video until the user presses a key.
 */
void Video::play() {

	getPlayer().play();
}

VideoPlayer Video::getPlayer() {
	VideoPlayer player;
	Video *video = this;
	player.setVideo(video);

	return player;
}


void Video::applyFrameProcessor(FrameProcessor &processor) {
	for(auto it = frames.begin(); it != frames.end(); it++) {
		processor.processFrame(this, (*it));
	}
}

void Video::applyDoubleFrameProcessor(DoubleFrameProcessor &processor) {
	auto it = frames.begin();
	processor.processStart(this, *it);
	Frame* last = *it; it++;
	while(it != frames.end()) {
		processor.processDoubleFrame(this, last, *it);
		last = *it; it++;
	}
	processor.processEnd(this, last);
}
