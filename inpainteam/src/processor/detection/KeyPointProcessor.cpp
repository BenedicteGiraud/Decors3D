/*
 * KeyPointProcessor.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include <cv.h>
#include <highgui.h>

#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "KeyPointProcessor.h"
#include "entities/Video.h"
#include "entities/Frame.h"

using namespace std;

KeyPointProcessor::KeyPointProcessor() {

}

KeyPointProcessor::~KeyPointProcessor() {

}

void addKeypoints(Frame* frame, vector<KeyPoint>* keypoints) {
	for(auto keypoint : *keypoints) {
		frame->keypoints.push_back(new ExtendedPoint(keypoint, frame));
	}

	SurfDescriptorExtractor extractor;
	Mat descriptors;
	extractor.compute(frame->image, *keypoints, descriptors);
	if(frame->rawDescriptors.rows == 0) {
		frame->rawDescriptors = descriptors;
		return;
	}

	vconcat(frame->rawDescriptors, descriptors, frame->rawDescriptors);

	int i=0;
	for(auto keypoint : frame->keypoints) {
		keypoint->descriptor = frame->rawDescriptors.row(i++);
	}
}

void KeyPointProcessor::processFrame(Video* video, Frame* frame, Mat* image, ProcessorCallback* callback) {
	if(frame->keypoints.size() == 0) {
		vector<KeyPoint> keypoints;
		SurfFeatureDetector surf(
				2, // hessianThreshold
				4, // nOctaves
				8, // nOctaveLayers
				true, // extended
				false // upright
				);
		surf.detect(frame->image, keypoints);
		addKeypoints(frame, &keypoints); //*/

		keypoints.clear();
		GFTTDetector gftt(
				10000, // maxCorners
				0.01, // qualityLevel
				4, // minDistance
				3, // blockSize
				true, // useHarrisDetector
				0.1); // k*/
		gftt.detect(frame->image, keypoints);
		addKeypoints(frame, &keypoints);

	}
}
