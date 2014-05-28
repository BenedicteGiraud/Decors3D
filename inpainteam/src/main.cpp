#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/nonfree/features2d.hpp>

#include "Video.h"

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

	int outputCodec = CV_FOURCC('X', 'V', 'I', 'D');

	Video video(inputFilename);
	video.play();

	/*Mat image;


	VideoWriter outputVideo;
	int width = (int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
	Size size = Size(width, height);
    outputVideo.open(outputDirectory + "/output.avi", outputCodec,
			inputVideo.get(CV_CAP_PROP_FPS), size, true);

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
