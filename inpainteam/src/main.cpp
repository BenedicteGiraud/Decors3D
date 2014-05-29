#include <iostream>
#include <cv.h>
#include <highgui.h>

#include "entities/Video.h"
#include "frame-annotator/KeyPointFrameAnnotator.h"
#include "frame-annotator/ResizeFrameAnnotator.h"
#include "frame-annotator/PipelineFrameAnnotator.h"

#include "frame-processor/DoubleFrameProcessor.h"
#include "frame-processor/KeyPointProcessor.h"

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

	struct : FrameProcessor {
		void processFrame(Video* video, Frame* frame) {
			for(auto point : frame->keypoints) {
				for(auto trace : video->traces) {

				}
			}
		}
	} processor;
	video.applyFrameProcessor(processor);

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

	KeyPointFrameAnnotator keypoints;

	PipelineFrameAnnotator pipeline;
	pipeline.add(&keypoints);
	pipeline.add(&resize);

	player.setFramesAnnotator(&pipeline);
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
