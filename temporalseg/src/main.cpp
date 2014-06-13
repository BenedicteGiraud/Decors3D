//Theophile Dalens 2014

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("usage: %s filename output [minute secondes] [length minute length secondes] [seuil 1] [seuil 2]", argv[0]);
		return -1;
	}

	VideoCapture movie(argv[1]);
	//VideoCapture movie("C:/Users/Amine/Documents/Projet SI381/Enter The Void/Enter The Void.avi");
	//VideoCapture movie("C:/series/house of cards/House.of.Cards.S01E01.720p.WEBrip.AAC2.0.x264-NTb.www.TuSerie.COM.mkv");
	//VideoCapture movie("C:/series/pilot.mp4");
	//VideoCapture movie("C:/series/The Walking Dead - 3x16 - Welcome to the Tombs.mp4");

	//The prefix of the output files. Directory MUST exist.
	string outputPath = argv[2];

	//The suffix of the output files. Changes the type of image
	string outputExt = ".jpg";

	//start of the movie

	double minute, sec;


	if (argc > 3)
	{
		minute = atoi(argv[3]);
		sec = atoi(argv[4]);
	}
	else
	{
		minute = 0.; sec = 0.;
	}
	//length of the movie

	double lenmin, lensec;
	if (argc > 5)
	{
		lenmin = atoi(argv[5]);
		lensec = atoi(argv[6]);
	}
	else
	{
		lenmin = 5., lensec = 30.;
	}

	//number of bins for the 3D HSV histogram
	int hSize = 8, sSize = 4, vSize = 4; int sizes[] = { hSize, sSize, vSize };

	//ratio of histogram intersection that says if two consecutive frames belong to same shot
	double histRatio;

	if (argc > 7)
	{
		histRatio = atof(argv[7]);
	}
	else
	{
		histRatio = 0.75;
	}

	//ratio of histogram intersection that says if two frames describe same point of view
	double histKeyRatio;

	if (argc > 8)
	{
		histKeyRatio = atof(argv[8]);
	}
	else
	{
		histKeyRatio = 0.87;
	}
	cout << histKeyRatio;
	//------------code

	if (!movie.isOpened())
		return -1;

	movie.set(CV_CAP_PROP_POS_MSEC, 1000.*(minute*60. + sec));
	double fr = movie.get(CV_CAP_PROP_POS_FRAMES);
	ofstream info;
	info.open(outputPath + "info.txt");

	int channels[] = { 0, 1, 2 };

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


	//The ranges of the histogramss
	float hRange[] = { 0.f, 180.f };
	float sRange[] = { 0.f, 256.f };
	float vRange[] = { 0.f, 256.f };
	const float* ranges[] = { hRange, sRange, vRange };

	movie >> frame;
	cvtColor(frame, hsvframe, CV_BGR2HSV);
	//cout << frame.rows * frame.cols << endl;
	calcHist(&hsvframe, 1, channels, Mat(), hist, 3, sizes, ranges);


	int shotNumber = 0; 
	int key = 0;
	cout << "shot " << shotNumber << "\tkey " << key << endl;
	stringstream imPath;
	imPath << outputPath << setw(4) << setfill('0') << shotNumber << "-" << setw(4) << setfill('0') << key << outputExt;
	imwrite(imPath.str(), frame);
	info << shotNumber << " " << key << " " << fr++ << endl;
	double histThresh = histRatio * frame.rows * frame.cols;
	double histKeyThresh = histKeyRatio * frame.rows * frame.cols;
	hist.copyTo(histKey);
	movie >> frame;

	double endFrame = (60 * lenmin + lensec)*movie.get(CV_CAP_PROP_FPS) + fr;
	

	while (!frame.empty())
	{
		hist.copyTo(hist_prev);
		imshow("movie", frame);
		cvtColor(frame, hsvframe, CV_BGR2HSV);
		calcHist(&hsvframe, 1, channels, Mat(), hist, 3, sizes, ranges);
		double comp = compareHist(hist_prev, hist, CV_COMP_INTERSECT);
		if (comp < histThresh)
		{
			shotNumber++;
			key = 0;
			cout << "shot " << shotNumber << "\tkey " << key << endl;
			hist.copyTo(histKey);
			//if(waitKey()==27) break;
			stringstream imPath;
			imPath << outputPath << setw(4) << setfill('0') << shotNumber << "-" << setw(4) << setfill('0') << key << outputExt;
			imwrite(imPath.str(), frame);
			info << shotNumber << " " << key << " " << fr++ << endl;

		}
		else
		{
			double comp2 = compareHist(histKey, hist, CV_COMP_INTERSECT);
			if (comp2 < histKeyThresh)
			{
				key++;
				cout << "shot " << shotNumber << "\tkey " << key << endl;
				hist.copyTo(histKey);
				//if(waitKey()==27) break;
				stringstream imPath;
				imPath << outputPath << setw(4) << setfill('0') << shotNumber << "-" << setw(4) << setfill('0') << key << outputExt;
				imwrite(imPath.str(), frame);
				info << shotNumber << " " << key << " " << fr++ << endl;

			}
			//cout << comp << endl;
		}
		if (waitKey(1) == 27)
			break;
		movie >> frame;
	}
	return 0;

}