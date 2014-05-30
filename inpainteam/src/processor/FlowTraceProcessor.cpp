/*
 * FlowTraceProcessor.cpp
 *
 *  Created on: 2014/05/29
 *      Author: tr
 */

#include <cv.h>

#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/PointTrace.h"
#include "entities/ExtendedPoint.h"

#include "FlowTraceProcessor.h"

using namespace cv;
using namespace std;

FlowTraceProcessor::FlowTraceProcessor() {

}

FlowTraceProcessor::~FlowTraceProcessor() {

}

void extractDescriptorSURF(Frame* frame, Point2f pt, int delta, Mat &descriptor) {
	// calculate SURF descriptor
	vector<KeyPoint> keypoints;
	keypoints.push_back(KeyPoint(pt, delta));
	SurfDescriptorExtractor extractor;
	extractor.compute(frame->image, keypoints, descriptor);
}

/*void extractDescriptorPixel(Frame* frame, Point2f pt, int delta, Mat &descriptor) {
	// TODO border treatment
	Point_<int> p(pt.x, pt.y);
	int channels = frame->image.channels();

	if(p.x-delta < 0 || p.y-delta < 0 ||
			p.x+delta >= frame->image.cols || p.y+delta >= frame->image.rows) {
		return;
	}

	/*Mat result(1, (2*delta+1)*(2*delta+1), frame->image.type());

	int colResult = 0;
	for(int col=(p.x-delta)*channels; col<(p.x+delta)*channels; col++) {
		int rowResult = 0;
		for(int row=p.y-delta; row<p.y+delta; row++) {
			*result.ptr(rowResult, colResult) = *frame->image.ptr(row, col);
			rowResult++;
		}
		colResult++;
	}

	descriptor = result;*/
/*	int x = p.x-delta, y = p.y-delta, width = 2*delta+1, height = width;
	Mat c = frame->image(Rect(x,y,width,height));

	c = c.clone();
	descriptor = c.reshape(0, 1);
}*/

void extractDescriptors(Frame* frame, vector<Point2f> grid, vector<unsigned char> &status, int delta) {
	if(delta < 1) delta = 1;
	// extract descriptors, remove points if no descriptor was calculated
	Mat descriptors;
	int i=-1;
	for(Point2f pt : grid) {
		i++;
		if(status.at(i) == 0) continue;
		Mat singleDescriptor;
		extractDescriptorSURF(frame, pt, delta, singleDescriptor);
		if(singleDescriptor.rows == 0) {
			status.at(i) = 0;
			continue;
		}
		if(descriptors.rows == 0) {
			descriptors = singleDescriptor;
		}
		else {
			vconcat(descriptors, singleDescriptor, descriptors);
		}
	}

	frame->rawDescriptors = descriptors;
}

void FlowTraceProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
	frame2->keypoints.clear();
	frame2->rawKeypoints.clear();

	int delta = 10;
	// initial grid
	vector<Point2f> grid1;
	vector<PointTrace*> traces;
	if(video->pointTraces.size() == 0) {
		vector<unsigned char> status;
		for(int row=delta/2; row<frame1->image.rows; row += delta) {
			for(int col=delta/2; col<frame1->image.cols; col += delta) {
				Point2f p(col, row);
				grid1.push_back(p);
				status.push_back(1);
			}
		}
		extractDescriptors(frame1, grid1, status, delta);

		int statusIndex=-1;
		int descIndex=-1;
		for(auto it = grid1.begin(); it != grid1.end(); it++) {
			statusIndex++;
			if(status.at(statusIndex) == 0) {
				it = grid1.erase(it)-1;
				status.erase(status.begin() + statusIndex--);
				continue;
			}
			descIndex++;

			// add point to frame and corresponding trace
			Mat descriptor1 = frame1->rawDescriptors.row(descIndex);
			ExtendedPoint* ep = new ExtendedPoint(*it, frame1);
			ep->descriptor = descriptor1;
			frame1->keypoints.push_back(ep);

			PointTrace *trace = new PointTrace(video);
			traces.push_back(trace);
			trace->points.push_back(ep);
			video->pointTraces.push_back(trace);
		}
	}
	else {
		for(PointTrace* trace : video->pointTraces) {
			ExtendedPoint* ep = trace->filter(frame1);
			if(ep != NULL) {
				traces.push_back(trace);
				grid1.push_back(ep->coordinates);
			}
		}
	}

	if(grid1.size() != traces.size()) {
		cout << "grid1.size() " << grid1.size() << " traces.size() " << traces.size() << endl;
		throw 1;
	}

	// caculate next point with Lucas-Kanade
	vector<Point2f> grid2 = grid1;
	vector<unsigned char> status;
	vector<float> err;

	if(grid1.size() == 0 || grid2.size() == 0) {
		cout << "Warning: FlowTraceProcessor didn't find any points for this frame";
		return;
	}
	calcOpticalFlowPyrLK(
			frame1->image, frame2->image, // images
			grid1, grid2, // initial points and estimation
			status, // whether flow is valid for each point
			err); // indicates quality of each point?

	extractDescriptors(frame2, grid2, status, delta);
	if(frame2->rawDescriptors.rows == 0) {
		return;
	}

	Mat averageDescriptor;
	cv::reduce(frame2->rawDescriptors, averageDescriptor, 0, CV_REDUCE_AVG, -1);
	repeat(averageDescriptor, frame2->rawDescriptors.rows, 1, averageDescriptor);
	Mat maxDiffDescriptor;
	cv::reduce(abs(frame2->rawDescriptors - averageDescriptor), maxDiffDescriptor, 0, CV_REDUCE_MAX, -1);

	// add points to traces, if they match
	auto itTrace = traces.begin();
	int statusIndex=-1;
	int descIndex = -1;
	for(Point2f pt : grid2) {
		statusIndex++;
		if(status.at(statusIndex) == 0) {
			itTrace++;
			continue;
		}
		descIndex++;
		ExtendedPoint* lastEp = (*itTrace)->points.front();
		if(lastEp == NULL) {
			itTrace++;
			continue;
		}
		Mat descriptor1 = lastEp->descriptor;
		Mat descriptor2 = frame2->rawDescriptors.row(descIndex);

		Mat scaled = descriptor1-descriptor2;
		for(int col=0; col<scaled.cols; col++) {
			//scaled.at<float>(0, col) /= maxDiffDescriptor.at<float>(0, col);
		}
		double ratio = norm(scaled);
		if(ratio > 0.70) {
			itTrace++;
			continue;
		}

		// add point to frame and corresponding trace
		ExtendedPoint* ep = new ExtendedPoint(pt, frame2);
		ep->descriptor = descriptor2;
		frame2->keypoints.push_back(ep);
		(*itTrace)->points.push_back(ep);
		itTrace++;
	}

	if(itTrace != traces.end()) {
		cout << "some traces not processed ... " << endl;
		throw 1;
	}
}
