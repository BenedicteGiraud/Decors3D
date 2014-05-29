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

KeyPointProcessor::KeyPointProcessor() {

}

KeyPointProcessor::~KeyPointProcessor() {

}

void KeyPointProcessor::processFrame(Video* video, Frame* frame) {
	if(frame->keypoints.size() == 0) {
		SurfFeatureDetector detector(
				2, // hessianThreshold
				4, // nOctaves
				8, // nOctaveLayers
				true, // extended
				false // upright
				); //*/

		/*GFTTDetector detector(
				10000, // maxCorners
				0.01, // qualityLevel
				4, // minDistance
				3, // blockSize
				true, // useHarrisDetector
				0.1); // k*/
		vector<KeyPoint> keypoints;
		detector.detect(frame->image, keypoints);

		for(auto keypoint : keypoints) {
			frame->keypoints.push_back(new ExtendedPoint(keypoint, frame));
		}

		SurfDescriptorExtractor extractor;
		extractor.compute(frame->image, keypoints, frame->rawDescriptors);

		int i=0;
		for(auto keypoint : frame->keypoints) {
			keypoint->descriptor = frame->rawDescriptors.row(i++);
		}
	}
}
