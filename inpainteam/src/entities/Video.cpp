
#include <highgui.h>

#include "processor/VideoProcessor.h"
#include "processor/DoubleFrameProcessor.h"
#include "processor/FrameProcessor.h"

#include "processor/ProcessorCallback.h"

#include "entities/Video.h"
#include "entities/Frame.h"

#include "Tools.h"


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

		frames.push_back(new Frame(image.clone(), this, index));
		index++;
	}
}

/**
 * Saves the video to a file using the XVID codec.
 * @param filename
 */
void Video::write(string filename) {
	write(filename, framesPerSecond);
}

/**
 * Saves the video to a file using the XVID codec.
 * @param filename
 */
void Video::write(string filename, double framesPerSecond) {
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


Video& Video::operator<<(const Mat& image){
	int index = 0;
	if(frames.size() > 0) {
		index = frames.back()->index;
	}
    Frame * frame = new Frame(image, this, index);
    frames.push_back(frame);

    return *this;
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

void Video::applyVideoProcessor(VideoProcessor &processor) {
	processor.process(this);
}

void Video::applyFrameProcessor(FrameProcessor &processor) {
	processor.processStart(this);
	for(auto it = frames.begin(); it != frames.end(); it++) {
		processor.processFrame(this, (*it), &(*it)->image, ProcessorCallback::getDefault());
	}
	processor.processEnd(this);
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

// TODO: check this function
bool Video::getHomography(Frame *from, Frame *to, Mat &homography) {
	bool inv = from->index > to->index;
	if(inv) {
		Frame* tmp = to;
		to = from; from = tmp;
	}

	if(to->homographyToLastFrame.rows == 0) return false;
	if(to->index - from->index == 1) {
		homography = to->homographyToLastFrame;
	}

	int minIndex = from->index, maxIndex = to->index;
	Mat result = Tools::getIdentityHomography();
	for(Frame* frame : frames) {
		if(frame->index > maxIndex) break;
		if(frame->index <= minIndex) continue;

		if(frame->homographyToLastFrame.rows == 0) return false;
		result = Tools::concatenateHomography(result, frame->homographyToLastFrame);
	}
	if(inv) {
		homography = result.inv(DECOMP_SVD);
	}
	else {
		homography=result;
	}
	return true;
}

void Video::applyDoubleFrameProcessorInverse(DoubleFrameProcessor &processor) {
    auto it = frames.rbegin();
    processor.processStart(this, *it);
    Frame* last = *it; it++;
    while(it != frames.rend()) {
        processor.processDoubleFrame(this, last, *it);
        last = *it; it++;
    }
    processor.processEnd(this, last);
}

map<PointTrace::Type, int> Video::getPointTraceCount() {
	typedef pair<PointTrace::Type, int> MapType;
	map<PointTrace::Type, int> result;
	result.insert(MapType(PointTrace::Type::unknown, 0));
	result.insert(MapType(PointTrace::Type::scene, 0));
	result.insert(MapType(PointTrace::Type::object, 0));

	for(PointTrace* trace : pointTraces) {
		if(trace != NULL) {
			result.find(trace->type)->second++;
		}
	}
	return result;
}

vector<PointTrace*> Video::filterPointTraces(PointTrace::Type type) {
	vector<PointTrace*> result;
	for(PointTrace* trace : pointTraces) {
		if(trace != NULL && trace->type == type) {
			result.push_back(trace);
		}
	}
	return result;
}
