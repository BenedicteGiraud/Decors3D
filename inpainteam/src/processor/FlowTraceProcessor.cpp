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
#include "Tools.h"

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

void extractDescriptors(Frame* frame,
		vector<Point2f> grid, Mat &descriptors,
		vector<unsigned char> &status, int delta) {
	if(delta < 1) delta = 1;
	// extract descriptors, remove points if no descriptor was calculated
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
		Tools::verticalConcatenateMatrices(descriptors, singleDescriptor, descriptors);
	}
}

vector<Point2f> sampleGrid(Frame* frame, vector<unsigned char> &status, int delta) {
	vector<Point2f> grid;
	for(int row=delta/2; row<frame->image.rows; row += delta) {
		for(int col=delta/2; col<frame->image.cols; col += delta) {
			Point2f p(col, row);
			grid.push_back(p);
			status.push_back(1);
		}
	}
	return grid;
}

vector<Point2f> resampleGrid(Frame* frame, vector<Point2f> points, vector<unsigned char> &status, int delta) {
	vector<Point2f> grid = sampleGrid(frame, status, delta);

	vector<Point2f> result;
	for(Point2f gridpoint : grid) {
		// check whether to add
		double minDist = FLT_MAX;
		for(Point2f point : points) {
			double dist = norm(gridpoint - point);
			if(dist < minDist) minDist = dist;
		}

		if(minDist >= delta/2) {
			result.push_back(gridpoint);
			status.push_back(1);
		}
	}

	return result;
}

void removeInvalidPoints(vector<Point2f> &points,
		vector<unsigned char> &status, vector<PointTrace*> &traces) {
	vector<Point2f> pointsFiltered;
	vector<unsigned char> statusFiltered;
	vector<PointTrace*> tracesFiltered;
	int statusIndex=-1;
	auto itTrace = traces.begin();
	for(auto point : points) {
		statusIndex++;
		if(status.at(statusIndex) != 0) {
			pointsFiltered.push_back(point);
			statusFiltered.push_back(1);
			if(traces.size() != 0) {
				tracesFiltered.push_back(*itTrace);
			}
		}
		if(traces.size() != 0) {
			itTrace++;
		}
	}
	points = pointsFiltered;
	status = statusFiltered;
	traces = tracesFiltered;

	if(points.size() != status.size()) {
		cout << "Warning: point size and status size not matching" << endl;
		throw 1;
	}
}

void addPointsToTraces(Video* video, Frame* frame,
		vector<Point2f> &points, vector<PointTrace*> &traces,
		vector<unsigned char> &status,
		bool (*checkAddPointToTrace)(PointTrace*, Point2f, Mat)) {

	cout << "Debug: addPointsToTraces," <<
			" frame: " << frame->index <<
			" points: " << points.size() <<
			" traces: " << traces.size() <<
			" status: " << status.size() <<
			" raw desc: " << frame->rawDescriptors.rows << endl;

	bool create = false;
	if(traces.size() == 0) {
		create = true;
	}

	if(status.size() > 0) {
		removeInvalidPoints(points, status, traces);
	}

	auto itTrace = traces.begin();
	int index=-1;
	for(auto it = points.begin(); it != points.end(); it++) {
		index++;

		// add point to frame and corresponding trace
		Mat descriptor = frame->rawDescriptors.row(index);
		if(checkAddPointToTrace != NULL &&
				!checkAddPointToTrace(*itTrace, *it, descriptor)) {
			if(!create) {
				itTrace++;
			}
			continue;
		}

		ExtendedPoint* ep = new ExtendedPoint(*it, frame);
		ep->descriptor = descriptor;
		frame->keypoints.push_back(ep);

		PointTrace *trace;
		if(create) {
			trace = new PointTrace(video);
			traces.push_back(trace);
			video->pointTraces.push_back(trace);
		}
		else {
			trace = *itTrace;
			itTrace++;
		}
		trace->points.push_back(ep);
	}

	if(!create && itTrace != traces.end()) {
		cout << "some traces not processed ... " << endl;
		throw 1;
	}
}

bool checkAddPointToTrace(PointTrace* trace, Point2f point, Mat descriptor) {
	ExtendedPoint* lastEp = trace->points.front();
	if(lastEp == NULL) {
		return false;
	}
	Mat descriptor1 = lastEp->descriptor;
	Mat descriptor2 = descriptor;

	Mat scaled = descriptor1-descriptor2;
	for(int col=0; col<scaled.cols; col++) {
		//scaled.at<float>(0, col) /= maxDiffDescriptor.at<float>(0, col);
	}
	double ratio = norm(scaled);
	return(ratio < 0.70);
}

void FlowTraceProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
	cout << "frame " << frame1->index;
	frame2->keypoints.clear();
	frame2->rawKeypoints.clear();

	int delta = 10;
	int descriptorScale = delta/2;
	// initial grid
	vector<Point2f> grid1;
	vector<PointTrace*> traces;
	if(video->pointTraces.size() == 0) {
		vector<unsigned char> status;
		grid1 = sampleGrid(frame1, status, delta);
		Mat descriptors;
		extractDescriptors(frame1, grid1, descriptors, status, descriptorScale);
		Tools::verticalConcatenateMatrices(frame1->rawDescriptors, descriptors, frame1->rawDescriptors);
		addPointsToTraces(video, frame1, grid1, traces, status, NULL);
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

	// TODO: filter with err
	cout << " status " << status.size() << " err " << err.size() << endl;
	for(int i=0; i<status.size(); i++) {
		cout << " " << err.at(i);
		if(err.at(i) > 6) status.at(i) = 0;
	}
	cout << endl;

	Mat descriptors;
	extractDescriptors(frame2, grid2, descriptors, status, descriptorScale);
	Tools::verticalConcatenateMatrices(frame2->rawDescriptors, descriptors, frame2->rawDescriptors);
	if(frame2->rawDescriptors.rows == 0) {
		return;
	}

	Mat averageDescriptor;
	cv::reduce(frame2->rawDescriptors, averageDescriptor, 0, CV_REDUCE_AVG, -1);
	repeat(averageDescriptor, frame2->rawDescriptors.rows, 1, averageDescriptor);
	Mat maxDiffDescriptor;
	cv::reduce(abs(frame2->rawDescriptors - averageDescriptor), maxDiffDescriptor, 0, CV_REDUCE_MAX, -1);

	// add points to traces, if they match
	addPointsToTraces(video, frame2, grid2, traces, status, checkAddPointToTrace);

	vector<unsigned char> additionalPointsStatus;
	Mat additionalDescriptors;
	vector<Point2f> additionalPoints = resampleGrid(frame2, grid2, additionalPointsStatus, delta/1.25);
	extractDescriptors(frame2, additionalPoints, additionalDescriptors, additionalPointsStatus, descriptorScale);
	Tools::verticalConcatenateMatrices(frame2->rawDescriptors, additionalDescriptors, frame2->rawDescriptors);
	vector<PointTrace*> dummyTraces;
	addPointsToTraces(video, frame2, additionalPoints, dummyTraces, additionalPointsStatus, NULL);
}
