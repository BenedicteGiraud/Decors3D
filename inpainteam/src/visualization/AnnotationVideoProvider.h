/*
 * AnnotationVideoProvider.h
 *
 *  Created on: 2014/06/05
 *      Author: tr
 */

#ifndef ANNOTATIONVIDEOPROVIDER_H_
#define ANNOTATIONVIDEOPROVIDER_H_

#include "visualization/VideoProvider.h"

class Video;
class FrameProcessor;

class AnnotationVideoProvider : public VideoProvider {
public:
	AnnotationVideoProvider(Video *video, FrameProcessor *annotator = NULL, Video *annotationData = NULL);
	virtual ~AnnotationVideoProvider();

	Video *video;
	FrameProcessor *annotator;
	Video *annotationData;
	int index;

	virtual void start();
	virtual void finish();

	virtual bool seek(int index);
	virtual bool seekRelative(int shift);

	virtual Mat getImage();
};

#endif /* ANNOTATIONVIDEOPROVIDER_H_ */
