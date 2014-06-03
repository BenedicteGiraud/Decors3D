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

}

TraceInterpolationProcessor::~TraceInterpolationProcessor() {

}

void TraceInterpolationProcessor::processStart(Video *video) {
	Frame* frame = video->frames.front();
	result = Mat::zeros(frame->image.size(), CV_32FC3);
	count = Mat(frame->image.size(), CV_32F, Scalar(0));

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

void TraceInterpolationProcessor::processFrame(Video* video, Frame* frame, cv::Mat* image) {
	Mat points(video->pointTraces.size(), 2, CV_32F);

	// fill up working list

	queue<WorkingItem*> workingitems;

	cout << "dbg A frame " << frame->index << endl;
	for(ExtendedPoint* ep : frame->keypoints) {
		if(ep == NULL || ep->trace == NULL) continue;
		WorkingItem *item = new WorkingItem;
		item->col = ep->coordinates.x;
		item->row = ep->coordinates.y;
		item->trace = ep->trace;
		item->center = ep;
		cout << " added " << item->id << ": (" << item->col << "," << item->row << ") center " << item->center << endl;
		workingitems.push(item);
	}

	unsigned int value = 0;
	Mat done(image->rows, image->cols, DataType<unsigned int>::type, Scalar(value));

	cout << "dbg B frame " << frame->index << endl;
	while(!workingitems.empty()) {
		WorkingItem* work = workingitems.front(); workingitems.pop();
		cout << "working list size " << workingitems.size() << endl;
		cout << "acessing " << work->center
				<< " because of working point " << work << ", " << work->id << ": "
				<< "(" << work->col << "," << work->row << ")"
				<< " center " << work->center << " trace " << work->trace << endl;
		if(work->id >= WorkingItem::nextid) {
			cout << "WTF???" << endl;
		}
		int distanceX = work->col-(work->center->coordinates.x);
		int distanceY = work->row-(work->center->coordinates.y);
		unsigned distance = distanceX*distanceX + distanceY+distanceY;
		if(distance > 7*7) {
			cout << "deleted A " << work << endl;
			delete work;
			continue;
		}

		result.ptr<float>(work->row, work->col)[0] += image->ptr<uchar>(work->row,work->col)[0] / 255.0;
		result.ptr<float>(work->row, work->col)[1] += image->ptr<uchar>(work->row, work->col)[1] / 255.0;
		result.ptr<float>(work->row, work->col)[2] += image->ptr<uchar>(work->row, work->col)[2] / 255.0;
		count.at<float>(work->row, work->col) += 1;
		done.at<unsigned int>(work->row, work->col) = 1;

		// Maybe optimize with several dNeighbor arrays, check before adding to queue
		for(int i=0; i<8; i++) {
			int nrow = work->row + dNeighbor[i][0];
			int ncol = work->col + dNeighbor[i][1];

			if(nrow < 0 || ncol < 0) continue;
			if(nrow >= image->rows || ncol >= image->cols) continue;
			if(done.at<unsigned int>(nrow, ncol) == 1) continue;

			WorkingItem *newitem = new WorkingItem;
			newitem->col = ncol;
			newitem->row = nrow;
			newitem->center = work->center;
			newitem->trace = work->trace;
			cout << "workingpoint " << work << ", " << work->id << ": (" << work->col << "," << work->row << ") center " << work->center
					<< " added " << newitem << ", " << newitem->id << ": (" << newitem->col << "," << newitem->row << ") center " << newitem->center << endl;
			workingitems.push(newitem);
			done.at<unsigned int>(nrow, ncol) = 1;
		}
		cout << "deleted B " << work << endl;
		delete work;
	}
	cout << "dbg C frame " << frame->index << endl;
}

Mat TraceInterpolationProcessor::getImage() {
	Mat result = this->result.clone();

	for(int row=0; row<result.rows; row++) {
		float* ptr = result.ptr<float>(row);
		float* countptr = count.ptr<float>(row);
		for(int col=0; col<result.cols; col++) {
			for(int channel=0; channel<result.channels(); channel++) {
				*ptr = *ptr / *countptr;
				ptr++;
			}
			countptr++;
		}
	}

	return result;
}


Mat TraceInterpolationProcessor::getCountImage() {
	Mat result = this->count.clone();
	int max = INT_MIN;
	for(int row=0; row<result.rows; row++) {
		for(int col=0; col<result.cols; col++) {
			if(result.at<float>(row,col) > max) max = result.at<float>(row,col);
		}
	}

	cout << "MAX VALUE " << max << endl;

	for(int row=0; row<result.rows; row++) {
		for(int col=0; col<result.cols; col++) {
			result.at<float>(row,col) = result.at<float>(row,col) / max;
		}
	}
	return result;
}
