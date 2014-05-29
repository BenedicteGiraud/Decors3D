#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/features2d/features2d.hpp>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "frame-annotator/TraceFrameAnnotator.h"
#include "frame-annotator/ResizeFrameAnnotator.h"
#include "frame-annotator/PipelineFrameAnnotator.h"
#include "frame-annotator/HomographyAnnotator.h"

#include "frame-processor/DoubleFrameProcessor.h"
#include "frame-processor/KeyPointProcessor.h"
#include "frame-processor/CombineProcessor.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	if( argc < 3) {
		cout << "Missing arguments: ";
        cout << argv[0] << " [input] [output folder]\n";
		return -1;
	}
	string inputFilename = argv[1];
	string outputDirectory = argv[2];

	Video video(inputFilename);
	VideoPlayer player = video.getPlayer();

	KeyPointProcessor keypointProcessor;
	video.applyFrameProcessor(keypointProcessor);

	struct : DoubleFrameProcessor {
		int searchDistance = 10;

		void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
			// calculate matches between frame

			BFMatcher matcher;
			vector<vector<DMatch>> matches;
			matcher.radiusMatch(frame1->rawDescriptors, frame2->rawDescriptors, matches, searchDistance, Mat(), true);

			vector<Point2f> points1;
			vector<Point2f> points2;
			for(vector<DMatch> matchList : matches) {
				DMatch match = matchList.front();
				ExtendedPoint* ep1 = frame1->keypoints.at(match.queryIdx);
				ExtendedPoint* ep2 = frame2->keypoints.at(match.trainIdx);

				Point2f p1 = ep1->keypoint.pt;
				points1.push_back(p1);
				Point2f p2 = ep2->keypoint.pt;
				points2.push_back(p2);

				double distance = norm(p1 - p2);
				if(distance < searchDistance) {
					PointTrace* trace = ep1->getOrCreate();
					trace->points.push_back(ep2);
					ep2->trace = trace;
				}
			}

			// TODO: use scene / object information
			Mat homography = findHomography(points1, points2, CV_RANSAC);
			video->homographies.push_back(homography);

			// assign remaining points to traces
			vector<ExtendedPoint*> workingList(frame2->keypoints);
			for(auto it = workingList.begin(); it != workingList.end(); it++) {
				if((*it)->trace != NULL) continue;
				bool found = false;
				int minDistance = INT_MAX;
				PointTrace *bestTrace = NULL;
				for(auto trace = video->pointTraces.begin(); trace != video->pointTraces.end(); trace++) {
					// search for corresponding trace
					if((*trace)->points.size() < 1) continue;
					ExtendedPoint* last = (*trace)->points.back();

					Point2f projectedPoint = ExtendedPoint::applyHomography(homography,last->keypoint.pt);
					double distance = norm(projectedPoint - (*it)->keypoint.pt);
					if(distance < minDistance) {
						minDistance = distance;
						bestTrace = (*trace);
					}
				}

				if(minDistance < searchDistance) {
					bestTrace->points.push_back(*it);
					(*it)->trace = bestTrace;
					found = true;
					break;
				}

				if(!found) {
					// create new trace
					//(*it)->getOrCreate();
				}
			}
		}
	} processor;
	video.applyDoubleFrameProcessor(processor);

	CombineProcessor combineProcessor;
	video.applyFrameProcessor(combineProcessor);

	/*
	struct : DoubleFrameProcessor {
		void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
			frame1->image -= frame2->image;
		}

		void processEnd(Video* video, Frame* lastFrame) {

		}

		void processStart(Video* video, Frame* startFrame) {

		}
	} processor;
	video.applyDoubleFrameProcessor(processor);*/

	ResizeFrameAnnotator resize;
	resize.setFactor(3);

	TraceFrameAnnotator traceAnnotator;
	HomographyAnnotator homographyAnnotator;

	PipelineFrameAnnotator pipeline;
	//pipeline.add(&traceAnnotator);
	pipeline.add(&homographyAnnotator);

	pipeline.add(&resize);
	player.setFramesAnnotator(&pipeline);
	player.setFramesPerSecond(5);
	player.play();

	//video.write(outputDirectory + "/output.avi");

	/*Mat image;


	Mat mask, lastImage;
	vector<Mat> frames;
	while(true) {
		inputVideo >> image;
		if(image.empty()) break;

		int minHessian;
		SurfFeatureDetector detector(minHessian);
		vector<KeyPoint> keypoints;
		detector.detect(image, keypoints);
		Mat annotatedImage;
		drawKeypoints(image, keypoints, annotatedImage, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

		frames.push_back(annotatedImage);
		outputVideo << annotatedImage;
		if(lastImage.data) {
			if(mask.data) {
				mask += abs(image - lastImage);
			}
			else {
				mask = abs(image - lastImage);
			}
		}
		lastImage = image.clone();
	}

	double minVal, maxVal;
	minMaxLoc( mask, &minVal, &maxVal);
	mask = mask * (255 / maxVal);

	Mat result = 0*mask;
	for(int row=0; row<mask.rows; row++) {
		uchar* pMask = mask.ptr(row);
		uchar* pResult = result.ptr(row);
		for(int col=0; col<mask.cols; col++) {
			uchar v = *pMask + *(pMask+1) + *(pMask+2);

			if(v < 100) *pResult = 255;

			pMask+=3;
			pResult+=3;
		}
	}

	cout << minVal << ", " << maxVal << endl;

	namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
	imshow( "Display Image", result );
	waitKey(0);*/

	return 0;
}
