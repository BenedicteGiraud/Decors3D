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

	Mat image;
	while(true) {
		inputVideo >> image;
		if(image.empty()) break;

		frames.push_back(Frame(image.clone()));
	}
}

void Video::play() {
	play(24);
}

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
