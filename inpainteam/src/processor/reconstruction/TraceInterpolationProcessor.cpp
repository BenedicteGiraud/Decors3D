/*
 * TraceInterpolationProcessor.cpp
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
#include "TraceInterpolationProcessor.h"

#include "Tools.h"

using namespace cv;
using namespace std;

TraceInterpolationProcessor::TraceInterpolationProcessor() {
	debugVideo = NULL;
}

TraceInterpolationProcessor::~TraceInterpolationProcessor() {

}

typedef unsigned int CountType;
typedef unsigned int DistanceType;
typedef unsigned int InternType;
typedef uchar ImageType;

void TraceInterpolationProcessor::processStart(Video *video) {
	Frame* frame = video->frames.front();
	summedInterpolation = Mat_<Vec<InternType, 3>>(frame->image.size(), Vec<InternType, 3>(0,0,0));
	count = Mat_<Vec<CountType, 1>>(frame->image.size(), 0);

	debugVideo = new Video();
}

char dNeighbor[8][2] = {{0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}};


struct WorkingItem {
	static int nextid;
	int id = nextid++;
	int col;
	int row;
	//PointTrace* trace;
	ExtendedPoint* center;
	int centerX;
	int centerY;
	unsigned distance;
} ;
int WorkingItem::nextid = 0;

void TraceInterpolationProcessor::processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback) {
	Mat points(video->pointTraces.size(), 2, CV_32F);

	// fill up working list

	queue<WorkingItem*> workingitems;
	WorkingItem::nextid = 0;

	DistanceType value = UINT_MAX;
	Mat distanceMat(image->rows, image->cols, DataType<DistanceType>::type);
	for(int row=0; row<image->rows; row++) {
		DistanceType* ptr = distanceMat.ptr<DistanceType>(row,0);
		for(int col=0; col<image->cols; col++) {
			*ptr++ = value;
		}
	}

	for(ExtendedPoint* ep : frame->keypoints) {
		if(ep == NULL || ep->trace == NULL) continue;
		WorkingItem *item = new WorkingItem;
		item->col = ep->coordinates.x;
		item->row = ep->coordinates.y;
		if(item->col < 0 || item->row < 0 || item->col >= image->cols || item->row >= image->rows) continue;
		item->center = ep;
		item->centerX = ep->coordinates.x;
		item->centerY = ep->coordinates.y;
		item->distance = 0;
		workingitems.push(item);
		distanceMat.at<DistanceType>(item->row, item->col) = 0;
	}

	cout << "dbg B frame " << frame->index << endl;
	while(!workingitems.empty()) {
		WorkingItem* work = workingitems.front(); workingitems.pop();
		if(work == NULL) continue;
		unsigned distance = work->distance;
		if(distanceMat.at<DistanceType>(work->row, work->col) < distance) {
			delete work;
			continue;
		}
		/*if(distance > 40*40) {
			delete work;
			continue;
		}*/
		if(distanceMat.at<DistanceType>(work->row, work->col) > distance) {
			distanceMat.at<DistanceType>(work->row, work->col) = distance;
		}

		if(work->center->trace == NULL) {
			cout << "weird stuff happened: trace of point is empty, but it should contain sth" << endl;
			continue;
		}
		if(work->center->trace->type == PointTrace::scene) {
			Point2f backProj(work->col, work->row);
			Mat homography;
			if(video->getHomography(frame, video->frames.front(), homography)) {
				Point2f newBackProj = Tools::applyHomography(homography, backProj);
				backProj = newBackProj;
			}
			Point backProjInt = backProj;

			if(backProjInt.x >= 0 && backProjInt.y >= 0 &&
					backProjInt.y < summedInterpolation.rows && backProjInt.x < summedInterpolation.cols) {
				summedInterpolation.at<Vec<InternType, 3>>(backProjInt) += image->at<Vec<ImageType, 3>>(work->row,work->col);
				count.at<CountType>(backProjInt) += 1;
			}
		}

		// Maybe optimize with several dNeighbor arrays, check before adding to queue
		for(int i=0; i<8; i++) {
			int nrow = work->row + dNeighbor[i][0];
			int ncol = work->col + dNeighbor[i][1];

			if(nrow < 0 || ncol < 0) continue;
			if(nrow >= image->rows || ncol >= image->cols) continue;
			int distanceX = ncol-(work->centerX);
			int distanceY = nrow-(work->centerY);
			unsigned distance = distanceX*distanceX + distanceY*distanceY;
			if(distanceMat.at<DistanceType>(nrow, ncol) <= distance) continue;

			WorkingItem *newitem = new WorkingItem;
			newitem->col = ncol;
			newitem->row = nrow;
			newitem->center = work->center;
			newitem->centerX = work->centerX;
			newitem->centerY = work->centerY;
			newitem->distance = distance;
			workingitems.push(newitem);
			distanceMat.at<DistanceType>(nrow, ncol) = distance;
		}
		delete work;

		if(WorkingItem::nextid > 2*image->rows*image->cols) {
			cout << " sth is wrong ..." << endl;
			exit(0);
			return;
		}
	}

	debugVideo->frames.push_back(new Frame(getImage(), video, debugVideo->frames.size()));
}

Mat TraceInterpolationProcessor::getImage() {
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


Mat TraceInterpolationProcessor::getCountImage() {
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
