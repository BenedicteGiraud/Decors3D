/*
 * AnnotationVideoProvider.cpp
 *
 *  Created on: 2014/06/05
 *      Author: tr
 */

#include "entities/Video.h"
#include "entities/Frame.h"
#include "processor/FrameProcessor.h"

#include "AnnotationVideoProvider.h"

AnnotationVideoProvider::AnnotationVideoProvider(Video* video, FrameProcessor* annotator, Video* annotationData) {
	index = 0;
	this->video = video;
	this->annotator = annotator;
	this->annotationData = annotationData;
}

AnnotationVideoProvider::~AnnotationVideoProvider() {

}

void AnnotationVideoProvider::seek(int index) {
	if(this->video->frames.size() == 0) {
		this->index = 0;
	}
	else {
		while(index < 0) {
			// q&d
			index += this->video->frames.size();
		}
		this->index = index % this->video->frames.size();
	}
}

void AnnotationVideoProvider::seekRelative(int shift) {
	this->seek(this->index + shift);
}

Mat AnnotationVideoProvider::getImage() {
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
