//Theophile Dalens 2014

#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

int main(int, char**)
{
	VideoCapture movie("C:/Downloads/FILMS/The Hunt [Jagten] [2012] 720p BRRip H264 AC3 - CODY.mp4");
	//VideoCapture movie("C:/series/pilot.mp4");
	//VideoCapture movie("C:/series/The Walking Dead - 3x16 - Welcome to the Tombs.mp4");

	float range[] = {0.f, 256.f};	
	const float* ranges = {range};
	int histSize = 8;
	double histThresh = 790000.;
	double min = 12.;
	double sec = 32.;

	if(!movie.isOpened())
		return -1;

	movie.set(CV_CAP_PROP_POS_MSEC,1000.*(min*60.+sec));
	
	Mat frame, gray;
	Mat hist, hist_prev;
	
	movie >> frame;
	cvtColor(frame, gray, CV_BGR2GRAY);
	calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &ranges);
	for(;;)
	{
		hist.copyTo(hist_prev);
		movie >> frame;
		cvtColor(frame, gray, CV_BGR2GRAY);
		calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &ranges);
		double comp = compareHist(hist_prev, hist, CV_COMP_INTERSECT);
		cout << comp << endl;
		imshow("movie", frame);

		if(waitKey(comp>histThresh)==27)
			break;
	}
	return 0;
}