/*
 * VideoPlayer.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <vector>
#include <highgui.h>

#include "entities/Video.h"
#include "entities/Frame.h"

#include "processor/FrameProcessor.h"
#include "processor/ProcessorCallback.h"

#include "visualization/VideoProvider.h"
#include "visualization/AnnotationVideoProvider.h"
#include "VideoPlayer.h"

using namespace std;
using namespace cv;

VideoPlayer::VideoPlayer() {
	framesPerSecond = 24;
	provider = NULL;
}

VideoPlayer::~VideoPlayer() {

}

void VideoPlayer::setVideo(Video* video) {
	// TODO: deal with memory leak
	this->provider = new AnnotationVideoProvider(video);
}

void VideoPlayer::setFramesPerSecond(int framesPerSecond) {
	this->framesPerSecond = framesPerSecond;
}

void mouseCallbackFunction(int event, int x, int y, int flags, void* userdata) {
	((VideoProvider*)userdata)->mouseEventCallback(event, x, y, flags);
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

	provider->start();
	string windowName = "Video";
	namedWindow(windowName, WINDOW_NORMAL);
	setMouseCallback(windowName, mouseCallbackFunction, provider);

	while(true) {
		imshow("Video" , provider->getImage());

		int key;
		if(pause) {
			key = waitKey(0);
		}
		else {
			key = waitKey(delay);
		}
		if(key >= 0) {
			switch(key) {
			case 'q':
				exit(0);
				break;

			case ' ':
				pause = !pause;
				break;

			case 65361: // left
				pause = true;
				provider->seekRelative(-1);
				break;

			case 65363: // right
				pause = true;
				provider->seekRelative(+1);
				break;

			default:
				cout << "VideoPlayer: received key code " << key << ", exiting" << endl;
				return;
			}
		}

		if(!pause) {
			provider->seekRelative(+1);
		}
	}
	provider->finish();
}
