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

void VideoPlayerProcessorCallback::refreshGui() {
	player->refresh();
}

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

void VideoPlayer::setVideoProvider(VideoProvider* provider) {
	// TODO: deal with memory leak
	this->provider = provider;
}

void VideoPlayer::setFramesPerSecond(int framesPerSecond) {
	this->framesPerSecond = framesPerSecond;
}

void VideoPlayer::setOutputDirectory(string outputDirectory) {
	this->outputDirectory = outputDirectory;
}

void mouseCallbackFunction(int event, int x, int y, int flags, void* userdata) {
	if(userdata == NULL) return;
	VideoPlayer* player = (VideoPlayer*)userdata;
	player->mouseEvent(event, x, y, flags);
}

void VideoPlayer::mouseEvent(int event, int x, int y, int flags) {
	provider->mouseEventCallback(event, x, y, flags, &callback);
}

int outputIndex;

void VideoPlayer::refresh() {
	imshow("Video" , provider->getImage());
}

/**
 * Plays the video until the user presses a key, with specified frame rate
 * @param framesPerSecond
 */
void VideoPlayer::play() {
	if(outputDirectory == "") outputDirectory = ".";
	int delay = 1000 / framesPerSecond;
	if(framesPerSecond > 0 && delay < 0) {
		delay = 1;
	}
	bool pause = false;

	provider->start();
	string windowName = "Video";
	namedWindow(windowName, WINDOW_NORMAL);
	setMouseCallback(windowName, mouseCallbackFunction, this);

	while(true) {
		refresh();

		int key;
		if(pause) {
			key = waitKey(0);
		}
		else {
			key = waitKey(delay);
		}
		if(key >= 0) {
			switch(key) {
			case 'h':
				cout << "keys:" << endl;
				cout << "q: quit" << endl;
				cout << "space: pause" << endl;
				cout << "w: write to file, starting from actual position" << endl;
				cout << "esc/enter: quit player" << endl << endl;
				cout << "left/right: previous/next frame" << endl;
				cout << "down/up: previous/next 20 frame" << endl;
				cout << "page down/up: previous/next 200 frame" << endl;
				break;

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

			case 65362: // up
				pause = true;
				provider->seekRelative(+20);
				break;

			case 65364: // down
				pause = true;
				provider->seekRelative(-20);
				break;

			case 65365: // page up
				pause = true;
				provider->seekRelative(-200);
				break;

			case 65366: // page down
				pause = true;
				provider->seekRelative(-200);
				break;

			case 65360: // home
				provider->seek(0);
				break;

			case 'w':
				{
					Video output;
					do {
						output << provider->getImage();
					} while (provider->seekRelative(+1) != false);

					std::stringstream sstm;
					sstm << (outputIndex++);
					string outputFile = outputDirectory + "/videoPlayer" + sstm.str() + ".avi";
					output.write(outputFile);
					cout << "wrote file to " << outputFile << endl;
				}
				break;

			case 10:
			case 27:
				goto FINISH;
				break;

			default:
				cout << "VideoPlayer: received key code " << key << ", don't know what to do" << endl;
			}
		}

		if(!pause) {
			bool successful = provider->seekRelative(+1);
			if(!successful) pause = true;
		}
	}
FINISH:
	provider->finish();
}
