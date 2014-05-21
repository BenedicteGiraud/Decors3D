//Theophile Dalens 2014

#include <iostream>
#include <iomanip>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

int main(int, char**)
{
	//filepath to the movie
	VideoCapture movie("C:/4a/Telecom/SI381/data/Fellowship31-32.m4v");
	//VideoCapture movie("C:/series/house of cards/House.of.Cards.S01E01.720p.WEBrip.AAC2.0.x264-NTb.www.TuSerie.COM.mkv");
	//VideoCapture movie("C:/series/pilot.mp4");
	//VideoCapture movie("C:/series/The Walking Dead - 3x16 - Welcome to the Tombs.mp4");
	
	//The prefix of the output files. Directory MUST exist.
	string outputPath = "C:/4a/Telecom/SI381/keyframes/Fellowship";

	//start of the movie
	double minute = 0., sec=0.;

	float hRange[] = {0.f, 180.f};
	float sRange[] = {0.f, 256.f};
	float vRange[] = {0.f, 256.f};
	const float* ranges[] = {hRange, sRange, vRange};
	
	//number of bins for the 3D HSV histogram
	int hSize = 8, rSize=4, vSize=4;
	int sizes[] = {hSize, rSize, vSize};
	int channels[] = {0, 1, 2};

	//ratio of histogram intersection that says if two consecutive frames belong to same shot
	double histRatio = 0.71;

	//ratio of histogram intersection that says if two frames describe same point of view
	double histKeyRatio = 0.84;

	if(!movie.isOpened())
		return -1;

	movie.set(CV_CAP_PROP_POS_MSEC,1000.*(minute*60.+sec));
	
	//the current frame
	Mat frame;
	
	//the current frame in HSV
	Mat hsvframe;

	//The current histogram
	Mat hist;
	
	//The previous histogram
	Mat hist_prev;

	//The histogram of the last key frame
	Mat histKey;

	movie >> frame;
	cvtColor(frame, hsvframe, CV_BGR2HSV);
	cout << frame.rows * frame.cols << endl;
	calcHist(&hsvframe, 1, channels, Mat(), hist, 3, sizes, ranges);


	int shotNumber = 0;
	int key = 0;
	cout << "shot " << shotNumber << "\tkey " << key << endl;
	stringstream imPath;
	imPath << outputPath << setw(2) << setfill('0') << shotNumber << "-" << setw(2) << setfill('0') << key << ".bmp";
	imwrite(imPath.str(), frame);
	double histThresh = histRatio * frame.rows * frame.cols;
	double histKeyThresh = histKeyRatio * frame.rows * frame.cols;
	hist.copyTo(histKey);
	


	while(!frame.empty())
	{
		hist.copyTo(hist_prev);
		movie >> frame;
		imshow("movie", frame);
		cvtColor(frame, hsvframe, CV_BGR2HSV);
		calcHist(&hsvframe, 1, channels, Mat(), hist, 3, sizes, ranges);
		double comp = compareHist(hist_prev, hist, CV_COMP_INTERSECT);
		if(comp<histThresh)
		{
			shotNumber++;
			key=0;
			cout << "shot " << shotNumber << "\tkey " << key << endl;
			hist.copyTo(histKey);
			//if(waitKey()==27) break;
			stringstream imPath;
			imPath << outputPath << setw(2) << setfill('0') << shotNumber << "-" << setw(2) << setfill('0') << key << ".bmp";
			imwrite(imPath.str(), frame);
		}
		else
		{
			double comp2=compareHist(histKey, hist, CV_COMP_INTERSECT);
			if(comp2<histKeyThresh)
			{
				key++;
				cout << "shot " << shotNumber << "\tkey " << key << endl;
				hist.copyTo(histKey);
				//if(waitKey()==27) break;
			stringstream imPath;
			imPath << outputPath << setw(2) << setfill('0') << shotNumber << "-" << setw(2) << setfill('0') << key << ".bmp";
			imwrite(imPath.str(), frame);
			}
		}
		//cout << comp << endl;

		if(waitKey(1)==27)
			break;
	}
	return 0;
}