/*
 * TraceInterpolationProcessor2.cpp
 *
 *  Created on: 2014/06/02
 *      Author: tr
 */

#include <cv.h>
#include <queue>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/flann/kdtree_index.h>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/PointTrace.h"
#include "entities/ExtendedPoint.h"
#include "TraceInterpolationProcessor2.h"

#include "Tools.h"

using namespace cv;
using namespace std;

TraceInterpolationProcessor2::TraceInterpolationProcessor2() {
	debugVideo = NULL;
}

TraceInterpolationProcessor2::~TraceInterpolationProcessor2() {

}

typedef double CountType;
struct DistanceType {
	unsigned int distance;
	int coordX;
	int coordY;
	ExtendedPoint* point;
};
struct PixelInformationType {
	bool done;
	vector<DistanceType> nearestKeypoints;
};
typedef double InternType;
typedef uchar ImageType;

void TraceInterpolationProcessor2::processStart(Video *video) {
	Frame* frame = video->frames.front();

	int frameCount = video->frames.back()->index;
	for(int i=0; i<frameCount; i++) {
		summedInterpolation.push_back(Mat_<Vec<InternType, 3>>(frame->image.size(), Vec<InternType, 3>(0,0,0)));
		count.push_back(Mat_<Vec<CountType, 1>>(frame->image.size(), 0));
	}

	debugVideo = new Video();
}

extern char dNeighbor[8][2];

struct WorkingItem {
	static int nextid;
	int id = nextid++;
	int col;
	int row;
	//PointTrace* trace;
	DistanceType center;
} ;

void TraceInterpolationProcessor2::processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback) {
	Mat points(video->pointTraces.size(), 2, CV_32F);

	// fill up working list
	queue<WorkingItem*> workingitems;
	WorkingItem::nextid = 0;
	PixelInformationType init;
	vector<DistanceType> initVec;
	init.done = false;
	vector<PixelInformationType> infoMat;
	int infoCols = image->cols;
	for(int row=0; row<image->rows; row++) {
		for(int col=0; col<image->cols; col++) {
			PixelInformationType info;
			info.done = false;
			infoMat.push_back(info);
		}
	}

	for(ExtendedPoint* ep : frame->keypoints) {
		if(ep == NULL || ep->trace == NULL) continue;
		WorkingItem *item = new WorkingItem;
		item->col = ep->coordinates.x;
		item->row = ep->coordinates.y;
		if(item->col < 0 || item->row < 0 || item->col >= image->cols || item->row >= image->rows) {
			delete item;
			continue;
		}
		item->center.distance = 0;
		item->center.point = ep;
		item->center.coordX = ep->coordinates.x;
		item->center.coordY = ep->coordinates.y;
		workingitems.push(item);

		PixelInformationType *info = &infoMat.at(item->row * infoCols + item->col);
		info->nearestKeypoints.push_back(item->center);
	}

	cout << "dbg B frame " << frame->index << endl;

	while(!workingitems.empty()) {
		WorkingItem* work = workingitems.front(); workingitems.pop();
		if(work == NULL) continue;
		PixelInformationType *info = &infoMat.at(work->row *infoCols + work->col);
		if(info->done) {
			delete work;
			continue;
		}
		info->done = true;

		// Maybe optimize with several dNeighbor arrays, check before adding to queue
		for(int i=0; i<8; i+=2) {
			int nrow = work->row + dNeighbor[i][0];
			int ncol = work->col + dNeighbor[i][1];

			if(nrow < 0 || ncol < 0) continue;
			if(nrow >= image->rows || ncol >= image->cols) continue;
			if(infoMat.at(nrow * infoCols + ncol).nearestKeypoints.size() > 3) continue;

			int distanceX = ncol-(work->center.coordX);
			int distanceY = nrow-(work->center.coordY);
			unsigned distance = distanceX*distanceX + distanceY*distanceY;

			// add new point to working list
			WorkingItem *newitem = new WorkingItem;
			newitem->col = ncol;
			newitem->row = nrow;
			newitem->center = work->center;
			newitem->center.distance = distance;
			workingitems.push(newitem);

			// actualize information
			PixelInformationType *info = &infoMat.at(nrow * infoCols + ncol);
			info->nearestKeypoints.push_back(newitem->center);
		}
		delete work;

		if(WorkingItem::nextid > 4*image->rows*image->cols) {
			cout << " sth is wrong ..." << endl;
			//exit(0);
			return;
		}
	}


	auto itSum = summedInterpolation.begin();
	auto itCount = count.begin();
	int frameToReconstruct = 0;
	for(; itSum!=summedInterpolation.end(), itCount != count.end(); itSum++, itCount++, frameToReconstruct++) {
		Mat homography;
		bool useHomography = video->getHomography(frame, *(video->frames.begin()+frameToReconstruct), homography);

		for(int row=0; row<image->rows; row++) {
			for(int col=0; col<image->cols; col++) {

				PixelInformationType *info = &infoMat.at(row * infoCols + col);
				bool doInterpolation = true;
				if(info->nearestKeypoints.size() == 0) doInterpolation = true;
				if(info->nearestKeypoints.size() > 0) {
					int pointScene = 0;
					int total = 0;
					double weighted = 0;
					double distanceSum = 0;
					for(DistanceType dist : info->nearestKeypoints) {
						if(dist.point->trace == NULL) {
							cout << "weird stuff happened: trace of point is empty, but it should contain sth" << endl;
							continue;
						}
						if(dist.point->trace->type == PointTrace::scene) {
							pointScene ++;
							weighted += dist.distance;
						}
						distanceSum += dist.distance;
					}
					doInterpolation &= pointScene >= 1;
					double epsilon = 0.0001;
					doInterpolation &= ((weighted+epsilon) / (distanceSum+epsilon) > 0.05);
				}

				if(doInterpolation) {
					Point2f backProj(col, row);
					if(useHomography) {
						Point2f newBackProj = Tools::applyHomography(homography, backProj);
						backProj = newBackProj;
					}
					Point backProjInt = backProj;

					if(backProjInt.x >= 0 && backProjInt.y >= 0 &&
							backProjInt.y < (*itSum).rows && backProjInt.x < (*itSum).cols) {
						int thisFrame = frame->index;
						int indexDiff = abs(frameToReconstruct - thisFrame);
						double weight = 1+exp(-indexDiff*indexDiff);
						Vec<InternType, 3> *ptr = &(*itSum).at<Vec<InternType, 3>>(backProjInt);
						Vec<InternType, 3> pixel = image->at<Vec<ImageType, 3>>(row,col);
						(*itSum).at<Vec<InternType, 3>>(backProjInt) += weight*pixel;
						(*itCount).at<CountType>(backProjInt) += weight;
					}
				}
			}
		}
	}

	debugVideo->frames.push_back(new Frame(getImage(), video, debugVideo->frames.size()));
}

Video TraceInterpolationProcessor2::getVideo() {
	Video result;

	auto itSum = summedInterpolation.begin();
	auto itCount = count.begin();
	for(; itSum!=summedInterpolation.end(), itCount != count.end(); itSum++, itCount++) {
		Mat normedResult = Mat::zeros(itSum->size(), CV_8UC3);

		for(int row=0; row<itSum->rows; row++) {
			Vec<ImageType,3> *ptr = normedResult.ptr<Vec<ImageType,3>>(row);
			CountType* countptr = itCount->ptr<CountType>(row);
			Vec<InternType,3> *summedInterpolationPtr = itSum->ptr<Vec<InternType,3>>(row);
			for(int col=0; col<itSum->cols; col++) {
				if(*countptr != 0) {
					*ptr = (*summedInterpolationPtr) / (*countptr);
				}
				else {
					*ptr = 0;
				}
				ptr++;
				summedInterpolationPtr++;
				countptr++;
			}
		}
		result << normedResult;
	}

	return result;
}

Mat TraceInterpolationProcessor2::getImage() {
	int baseFrame = summedInterpolation.size() / 2;
	Mat normedResult = Mat::zeros(summedInterpolation[baseFrame].size(), CV_8UC3);

	for(int row=0; row<summedInterpolation[baseFrame].rows; row++) {
		Vec<ImageType,3> *ptr = normedResult.ptr<Vec<ImageType,3>>(row);
		CountType* countptr = count[baseFrame].ptr<CountType>(row);
		Vec<InternType,3> *summedInterpolationPtr = summedInterpolation[baseFrame].ptr<Vec<InternType,3>>(row);
		for(int col=0; col<summedInterpolation[baseFrame].cols; col++) {
			if(*countptr != 0) {
				*ptr = (*summedInterpolationPtr) / (*countptr);
			}
			else {
				*ptr = 0;
			}
			ptr++;
			summedInterpolationPtr++;
			countptr++;
		}
	}

	return normedResult;
}


/*Mat TraceInterpolationProcessor2::getCountImage() {
	// TODO: rewrite
	Mat result = Mat::zeros(count.size(), CV_8U);
	long max = LONG_MIN;
	for(int row=0; row<result.rows; row++) {
		for(int col=0; col<result.cols; col++) {
			if(count.at<CountType>(row,col) > max) max = count.at<CountType>(row,col);
		}
	}

	cout << "MAX VALUE " << max << endl;

	for(int row=0; row<result.rows; row++) {
		for(int col=0; col<result.cols; col++) {
			result.at<ImageType>(row,col) = 255 * count.at<CountType>(row,col) / max;
		}
	}
	return result;
}*/
