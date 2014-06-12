/*
 * AnnotationVideoProvider.cpp
 *
 *  Created on: 2014/06/05
 *      Author: tr
 */
#include <iostream>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "processor/FrameProcessor.h"

#include "AnnotationVideoProvider.h"

using namespace std;

AnnotationVideoProvider::AnnotationVideoProvider(Video* video, FrameProcessor* annotator, Video* annotationData) {
	index = 0;
	this->video = video;
	this->annotator = annotator;
	if(annotationData == NULL) {
		this->annotationData = video;
	}
	else {
		this->annotationData = annotationData;
	}
}

AnnotationVideoProvider::~AnnotationVideoProvider() {

}

bool AnnotationVideoProvider::seek(int index) {
	if(this->video->frames.size() == 0) {
		this->index = 0;
		return false;
	}
	else {
		/*while(index < 0) {
			// q&d
			index += this->video->frames.size();
		}
		this->index = index % this->video->frames.size();*/
		if(this->video->frames.size() <= index || index < 0) {
			return false;
		}
		this->index = index;
		return true;
	}
}

bool AnnotationVideoProvider::seekRelative(int shift) {
	return this->seek(this->index + shift);
}

Mat AnnotationVideoProvider::getImage() {
    if (index >= video->frames.size()) {
        cout << "warning: AnnotationVideoProvider::getImage() VIDEO HAS NO FRAMES" << endl;
        return Mat();
    }
	Mat image = video->frames[index]->image;
	if(annotator != NULL) {
		image = image.clone();
		// TODO: implement processor callback
		Frame* annotationFrame = annotationData->frames[index];
		annotator->processFrame(annotationData, annotationFrame, &image, ProcessorCallback::getDefault());
	}
	return image;
}

void AnnotationVideoProvider::start() {
	if(annotator != NULL) {
		annotator->processStart(annotationData);
	}
}

void AnnotationVideoProvider::finish() {
	if(annotator != NULL) {
		annotator->processEnd(annotationData);
	}
}
