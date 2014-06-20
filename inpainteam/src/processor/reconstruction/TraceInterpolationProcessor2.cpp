/*
 * TraceInterpolationProcessor2.cpp
 *
 *  Created on: 2014/06/02
 *      Author: tr
 */

#include <cv.h>
#include <queue>

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

typedef unsigned int CountType;
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
typedef unsigned int InternType;
typedef uchar ImageType;

void TraceInterpolationProcessor2::processStart(Video *video) {
	Frame* frame = video->frames.front();
	summedInterpolation = Mat_<Vec<InternType, 3>>(frame->image.size(), Vec<InternType, 3>(0,0,0));
	count = Mat_<Vec<CountType, 1>>(frame->image.size(), 0);

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

	Mat homography;
	bool useHomography = video->getHomography(frame, video->frames.front(), homography);
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
						backProjInt.y < summedInterpolation.rows && backProjInt.x < summedInterpolation.cols) {
					summedInterpolation.at<Vec<InternType, 3>>(backProjInt) += image->at<Vec<ImageType, 3>>(row,col);
					count.at<CountType>(backProjInt) += 1;
				}
			}
		}
	}

	debugVideo->frames.push_back(new Frame(getImage(), video, debugVideo->frames.size()));
}

Mat TraceInterpolationProcessor2::getImage() {
	Mat normedResult = Mat::zeros(summedInterpolation.size(), CV_8UC3);

	for(int row=0; row<summedInterpolation.rows; row++) {
		ImageType* ptr = normedResult.ptr<ImageType>(row);
		CountType* countptr = count.ptr<CountType>(row);
		InternType* summedInterpolationPtr = summedInterpolation.ptr<InternType>(row);
		for(int col=0; col<summedInterpolation.cols; col++) {
			for(int channel=0; channel<summedInterpolation.channels(); channel++) {
				if(*countptr != 0) {
					ImageType value = *summedInterpolationPtr / *countptr;
					*ptr = value;
				}
				else {
					*ptr = 0;
				}
				ptr++;
				summedInterpolationPtr++;
			}
			countptr++;
		}
	}

	return normedResult;
}


Mat TraceInterpolationProcessor2::getCountImage() {
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
}
