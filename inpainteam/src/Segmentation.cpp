#include "Segmentation.h"

#include <iostream>
#include <cv.h>
#include <highgui.h>
using namespace std;
using namespace cv;

Segmentation::Segmentation(std::string filename,
		double seconde_,
		float histRatio_)
{
	movie = VideoCapture(filename);

	// Beginning of the sequence to extract
	histRatio = histRatio_;

	movie.set(CV_CAP_PROP_POS_MSEC, 1000.*(seconde_));
	goToNextFrame();
}



Video* Segmentation::nextSequence(){

	if (!movie.isOpened()){
		cout << "video could not be opened" << endl;
		return NULL;
	}

	// Creation of the Thresh
	double histThresh = histRatio * frame.rows * frame.cols;

	Video * video = new Video();
	int frames = 0;
	while (!movieEndReached)
	{
		frames++;
		(*video) << frame;
		movieEndReached = !goToNextFrame();
		double comp = compareHist(hist, nextHist, CV_COMP_INTERSECT);

		if (comp < histThresh)
		{
			break;
		}
	}
	if (frames == 0) {
		delete video;
		return NULL;
	}
	else {
		return video;
	}
}


bool Segmentation::goToNextFrame(){
	hist = nextHist.clone();
	frame = nextFrame.clone();

	movie >> frame;

	int channels[] = { 0, 1, 2 }; // Rot Grün Blau //

	// The ranges of the histogramss
	float hRange[] = { 0.f, 180.f };
	float sRange[] = { 0.f, 256.f };
	float vRange[] = { 0.f, 256.f };
	const float* ranges[] = { hRange, sRange, vRange };

	if (!frame.empty()) {
		// Calculate the histogramm for das hsv frame
		cvtColor(frame, hsvframe, CV_BGR2HSV);
		//cout << frame.rows * frame.cols << endl;
		calcHist(&hsvframe, 1, channels, Mat(), nextHist, 3, sizes, ranges);
		return true;
	}
	else {
		return false;
	}
}
