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
	PointTrace* trace;
	ExtendedPoint* center;
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
		item->trace = ep->trace;
		item->center = ep;
		workingitems.push(item);
		distanceMat.at<DistanceType>(item->row, item->col) = 0;
	}

	cout << "dbg B frame " << frame->index << endl;
	while(!workingitems.empty()) {
		WorkingItem* work = workingitems.front(); workingitems.pop();
		int distanceX = work->col-(work->center->coordinates.x);
		int distanceY = work->row-(work->center->coordinates.y);
		unsigned distance = distanceX*distanceX + distanceY*distanceY;
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

		if(work->trace->type == PointTrace::scene) {
			summedInterpolation.ptr<InternType>(work->row, work->col)[0] += image->ptr<ImageType>(work->row,work->col)[0];
			summedInterpolation.ptr<InternType>(work->row, work->col)[1] += image->ptr<ImageType>(work->row, work->col)[1];
			summedInterpolation.ptr<InternType>(work->row, work->col)[2] += image->ptr<ImageType>(work->row, work->col)[2];
			count.at<CountType>(work->row, work->col) += 1;
		}

		// Maybe optimize with several dNeighbor arrays, check before adding to queue
		for(int i=0; i<8; i++) {
			int nrow = work->row + dNeighbor[i][0];
			int ncol = work->col + dNeighbor[i][1];

			if(nrow < 0 || ncol < 0) continue;
			if(nrow >= image->rows || ncol >= image->cols) continue;
			int distanceX = ncol-(work->center->coordinates.x);
			int distanceY = nrow-(work->center->coordinates.y);
			unsigned distance = distanceX*distanceX + distanceY+distanceY;
			if(distanceMat.at<DistanceType>(nrow, ncol) <= distance) continue;

			WorkingItem *newitem = new WorkingItem;
			newitem->col = ncol;
			newitem->row = nrow;
			newitem->center = work->center;
			newitem->trace = work->trace;
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
