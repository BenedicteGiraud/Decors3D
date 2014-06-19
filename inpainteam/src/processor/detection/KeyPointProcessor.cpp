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
using namespace cv;

KeyPointProcessor::KeyPointProcessor() {

}

KeyPointProcessor::~KeyPointProcessor() {

}

double KeyPointProcessor::descriptorDistance(cv::Mat desc1, cv::Mat desc2) {
    double normalisationFactor = (double)1/(desc1.cols*desc1.channels()*256); // to give a result between 0 and 1
    return normalisationFactor*norm(desc1-desc2);
}

Ptr<DescriptorExtractor> getExtractor() {
	return DescriptorExtractor::create("SIFT");
}

// give the descriptor of the coordinates
// scale : factor between 0 and infini : give te scale of the descriptor
void KeyPointProcessor::extractPatchDescriptor(Mat image, Mat &descriptor, Point2f point, int halfSideLength, float scale) {

	/*int halfSideLength = 5;
	Mat descriptor(halfSideLength*2+1, halfSideLength*2+1, frame->image.depth());
	int top = -keypoint.pt.y+halfSideLength,
		bottom = -frame->image.rows+keypoint.pt.y+halfSideLength,
		left = -keypoint.pt.x+halfSideLength,
		right = -frame->image.cols+keypoint.pt.x+halfSideLength;
	copyMakeBorder(frame->image, descriptor, top, bottom, left, right, BORDER_REPLICATE);*/

	int size = halfSideLength * 2 + 1;
	descriptor = Mat(1, size*size*image.channels(), CV_8U);
	int i=0;
    for(int deltaRow=-halfSideLength; deltaRow<=halfSideLength; deltaRow++) {
        for(int deltaCol=-halfSideLength; deltaCol<=halfSideLength; deltaCol++) {
            int imageRow = borderInterpolate(point.y+scale*deltaRow, image.rows, BORDER_REPLICATE);
            int imageCol = borderInterpolate(point.x+scale*deltaCol, image.cols, BORDER_REPLICATE);
			descriptor.at<Vec<uchar, 3>>(0, i) =
					image.at<Vec<uchar,3>>(imageRow, imageCol);
			i++;
		}
	}

}

ExtendedPoint* KeyPointProcessor::addKeypoint(Frame* frame, Point2f point, Mat descriptor, ExtendedPoint::Detector detector) {

	if(descriptor.rows != 0) {
		ExtendedPoint* neighbor = frame->getNearestKeyPoint(point);
		if(neighbor != NULL) {
			if(norm(neighbor->coordinates - point) < 1) return NULL;
		}

		ExtendedPoint *ep = new ExtendedPoint(point, frame);
		frame->keypoints.push_back(ep);
		ep->descriptor = descriptor;
		ep->detector = detector;
		Tools::verticalConcatenateMatrices(frame->rawDescriptors, descriptor, frame->rawDescriptors);
		return ep;
	}
	return NULL;
}

void addKeypoint(Ptr<DescriptorExtractor> &extractor, Frame* frame, KeyPoint keypoint, ExtendedPoint::Detector detector) {
	//Mat descriptor;
	//vector<KeyPoint> keypoints;
	//keypoints.push_back(keypoint);
	//extractor->compute(frame->image, keypoints, descriptor);
	Mat descriptor;
	KeyPointProcessor::extractPatchDescriptor(frame->image, descriptor, keypoint.pt, 5, 2);
	KeyPointProcessor::addKeypoint(frame, keypoint.pt, descriptor, detector);
}

void addKeypoints(Frame* frame, vector<KeyPoint>* keypoints, ExtendedPoint::Detector detector) {
	Ptr<DescriptorExtractor> extractor = getExtractor();
	for(auto keypoint : *keypoints) {
		addKeypoint(extractor, frame, keypoint, detector);
	}
}

void addCannyPoints(Frame* frame) {
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.08;
	Mat channels[3];
	split(frame->image, channels);
	Mat dest;
	for(int i=0; i<3; i++) {
		Mat tmp;
		//cornerHarris( channels[i], tmp, blockSize, apertureSize, k, BORDER_DEFAULT );
		Canny(channels[i], tmp, 150, 200, 3);
		if(dest.rows == 0) {
			dest = tmp;
		}
		else {
			dest += tmp / 3;
		}
	}

	/*imshow("test", dest);
	waitKey(0);*/

	Ptr<DescriptorExtractor> extractor = getExtractor();
	for(int row=0; row<dest.rows; row++) {
		for(int col=0; col<dest.cols; col++) {
			if(dest.at<uchar>(row, col) > 200) {
				KeyPoint keypoint(col, row, 5);
				addKeypoint(extractor, frame, keypoint, ExtendedPoint::Canny);

				int sizer = min(5, dest.rows-row);
				int sizec = min(5, dest.cols-col);
				dest(Rect(col, row, sizec, sizer)) = Scalar(0);
			}
		}
	}
}

// Doesn't do anything usefull yet ...
void addHarrisPoints(Frame* frame) {
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	Mat channels[3];
	split(frame->image, channels);

	Mat dest;
	for(int i=0; i<3; i++) {
		/// Detecting corners
		Mat tmp;
		cornerHarris( channels[i], tmp, blockSize, apertureSize, k, BORDER_DEFAULT );
		if(dest.rows == 0) {
			dest = abs(tmp);
		}
		else {
			dest += abs(tmp);
		}
	}
	dest = dest.mul(dest);
	double min, max;
	minMaxLoc(dest, &min, &max, NULL, NULL);
	cout << "before: min " << min << " max " << max << endl;
	dest = 100*(dest - min) / (max - min);
	minMaxLoc(dest, &min, &max, NULL, NULL);
	cout << "after: min " << min << " max " << max << endl;
	/*namedWindow("test", WINDOW_NORMAL);
	imshow("test", dest);
	waitKey(0);*/
}

void KeyPointProcessor::processFrame(Video* video, Frame* frame, Mat* image, ProcessorCallback* callback) {
	if(frame->keypoints.size() == 0) {
		vector<KeyPoint> keypoints;
		SurfFeatureDetector surf(
				2, // hessianThreshold
				1, // nOctaves
				1, // nOctaveLayers
				true, // extended
				true // upright
				);
		surf.detect(frame->image, keypoints);
		addKeypoints(frame, &keypoints, ExtendedPoint::SURF); //*/

		keypoints.clear();
		GFTTDetector gftt(
				10000, // maxCorners
				0.01, // qualityLevel
				4, // minDistance
				3, // blockSize
				true, // useHarrisDetector
				0.001); // k
		gftt.detect(frame->image, keypoints);
		addKeypoints(frame, &keypoints, ExtendedPoint::GFTT); //*/

		// addCannyPoints(frame);
		// addHarrisPoints(frame);
	}
}
