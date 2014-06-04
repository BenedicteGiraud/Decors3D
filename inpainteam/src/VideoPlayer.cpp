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
#include "VideoPlayer.h"

#include "processor/FrameProcessor.h"
#include "processor/ProcessorCallback.h"

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

void VideoPlayer::setFramesAnnotator(FrameProcessor* annotator) {
	this->annotator = annotator;
}

/**
 * Plays the video until the user presses a key, with specified frame rate
 * @param framesPerSecond
 */
void VideoPlayer::play() {
	playWithAnnotationData(video);
}

void VideoPlayer::playWithAnnotationData(Video* annotationData) {
	int delay = 1000 / framesPerSecond;
	if(framesPerSecond > 0 && delay < 0) {
		delay = 1;
	}
	bool pause = false;

	if(annotator != NULL) {
		annotator->processStart(annotationData);
	}
	namedWindow("Video", WINDOW_NORMAL);

	auto it = video->frames.begin();
	auto annotationIt = annotationData->frames.begin();
	while(true) {
		if(!pause) {
			if(it == video->frames.end()) {
				it = video->frames.begin();
				annotationIt = annotationData->frames.begin();
			}
		}

		Mat image = (*it)->image;
		if(annotator != NULL) {
			image = image.clone();
			// TODO: implement processor callback
			annotator->processFrame(annotationData, *annotationIt, &image, ProcessorCallback::getDefault());
		}

		imshow("Video" , image);

		int key = waitKey(delay);
		if(key >= 0) {
			switch(key) {
			case ' ':
				pause = !pause;
				break;

			case 65361: // left
				pause = true;
				if(it == video->frames.begin()) {
					it = video->frames.end();
					annotationIt = annotationData->frames.end();
				}
				it--;
				annotationIt--;
				break;

			case 65363: // right
				pause = true;
				it++;
				annotationIt++;
				if(it == video->frames.end()) {
					it = video->frames.begin();
					annotationIt = annotationData->frames.begin();
				}
				break;

			default:
				cout << "VideoPlayer: received key code " << key << ", exiting" << endl;
				return;
			}
		}

		if(!pause) {
			it++;
			annotationIt++;
		}
	}
	if(annotator != NULL) {
		annotator->processEnd(annotationData);
	}
}
