#include "Video.h"

#include <highgui.h>

using namespace std;
using namespace cv;

Video::Video()
{
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
	while(true) {
		inputVideo >> image;
		if(image.empty()) break;

		frames.push_back(Frame(image.clone()));
	}
}

/**
 *Plays video until the user presses a key.
 */
void Video::play() {
	play(framesPerSecond);
}

/**
 * Plays the video until the user presses a key, with specified frame rate
 * @param framesPerSecond
 */
void Video::play(int framesPerSecond) {
	int delay = 1000 / framesPerSecond;
	if(framesPerSecond > 0 && delay < 0) {
		delay = 1;
	}

	vector<Frame>::iterator it = frames.begin();
	while(true) {
		if(++it == frames.end()) {
			it = frames.begin();
		}

		imshow("Video" , it->image);

		if(waitKey(delay) >= 0) {
			break;
		}
	}
}

/**
 * Saves the video to a file using the XVID codec.
 * @param filename
 */
void Video::write(string filename) {
	int outputCodec = CV_FOURCC('X', 'V', 'I', 'D');


	VideoWriter outputVideo;
	int width = (int) frames.begin()->image.size().width;
	int height = (int) frames.begin()->image.size().height;
	Size size = Size(width, height);
	outputVideo.open(filename, outputCodec,
			framesPerSecond, size, true);

	vector<Frame>::iterator it = frames.begin();
	for(; it != frames.end(); it++) {
		outputVideo << it->image;
	}
}
