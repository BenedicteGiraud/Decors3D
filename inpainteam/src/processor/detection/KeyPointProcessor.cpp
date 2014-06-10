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

#include "Tools.h"

using namespace std;

KeyPointProcessor::KeyPointProcessor() {

}

KeyPointProcessor::~KeyPointProcessor() {

}

void addKeypoint(Ptr<DescriptorExtractor> &extractor, Frame* frame, KeyPoint keypoint) {
	vector<KeyPoint> keypoints;
	keypoints.push_back(keypoint);
	Mat descriptors;
	extractor->compute(frame->image, keypoints, descriptors);

	if(descriptors.rows != 0) {
		ExtendedPoint *ep = new ExtendedPoint(keypoint, frame);
		frame->keypoints.push_back(ep);
		ep->descriptor = descriptors;
		Tools::verticalConcatenateMatrices(frame->rawDescriptors, descriptors, frame->rawDescriptors);
	}
}

void addKeypoints(Frame* frame, vector<KeyPoint>* keypoints) {
	Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SURF");
	for(auto keypoint : *keypoints) {
		addKeypoint(extractor, frame, keypoint);
	}
}

void KeyPointProcessor::processFrame(Video* video, Frame* frame, Mat* image, ProcessorCallback* callback) {
	if(frame->keypoints.size() == 0) {
		vector<KeyPoint> keypoints;
		SurfFeatureDetector surf(
				2, // hessianThreshold
				1, // nOctaves
				1, // nOctaveLayers
				true, // extended
				false // upright
				);
		surf.detect(frame->image, keypoints);
		addKeypoints(frame, &keypoints); //

		keypoints.clear();
		GFTTDetector gftt(
				10000, // maxCorners
				0.001, // qualityLevel
				4, // minDistance
				3, // blockSize
				true, // useHarrisDetector
				0.0001); // k*/
		gftt.detect(frame->image, keypoints);
		addKeypoints(frame, &keypoints);

	}
}
