/*
 * VideoPlayer.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <vector>
#include <highgui.h>

#include "VideoPlayer.h"
#include "entities/Frame.h"

using namespace std;
using namespace cv;

VideoPlayer::VideoPlayer() {
	video = NULL;
	framesPerSecond = 24;
	annotator = NULL;
}

VideoPlayer::~VideoPlayer() {

}

void VideoPlayer::setVideo(Video* video) {
	this->video = video;
}

void VideoPlayer::setFramesPerSecond(int framesPerSecond) {
	this->framesPerSecond = framesPerSecond;
}

void VideoPlayer::setFramesAnnotator(FrameAnnotator* annotator) {
	this->annotator = annotator;
}

/**
 * Plays the video until the user presses a key, with specified frame rate
 * @param framesPerSecond
 */
void VideoPlayer::play() {
	int delay = 1000 / framesPerSecond;
	if(framesPerSecond > 0 && delay < 0) {
		delay = 1;
	}
	bool pause = false;

	auto it = video->frames.begin();
	while(true) {
		if(!pause) {
			if(++it == video->frames.end()) {
				it = video->frames.begin();
			}
		}

		Mat image = (*it)->image;
		if(annotator != NULL) {
			image = image.clone();
			annotator->annotate(video, *it, &image);
		}

		imshow("Video" , image);

		int key = waitKey(delay);
		if(key >= 0) {
			switch(key) {
			case ' ':
				pause = !pause;
				break;

			default:
				cout << "VideoPlayer: received key code " << key << ", exiting" << endl;
				return;
			}
		}
	}
}
