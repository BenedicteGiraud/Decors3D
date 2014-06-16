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
	int offsetX = delta/2;
	int offsetY = delta/2;

	int csize = frame->image.cols / delta;
	int rsize = frame->image.rows / delta;
	vector<bool> hasNeighboringPoint(csize*rsize);
	hasNeighboringPoint.resize(csize*rsize, false);
	cout << "hNP size " << hasNeighboringPoint.size() << endl;
	for(Point2f point : points) {
		// determine next grid point
		int x = round((point.x-offsetX) / delta);
		x = (x < 0) ? 0 : ((x >= csize) ? (csize - 1) : x);
		int y = round((point.y-offsetY) / delta);
		y = (y < 0) ? 0 : ((y >= rsize) ? (rsize - 1) : y);

		hasNeighboringPoint[y * csize + x] = true;
	}

	vector<Point2f> result;
	int i=0;
	for(int y=0; y<rsize; y++) {
		for(int x=0; x<csize; x++) {
			if(!hasNeighboringPoint[i++]) {
				result.push_back(Point2f(x*delta+offsetX, y*delta + offsetY));
				status.push_back(1);
			}
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
		trace->addOrReplacePoint(ep);
	}

	if(!create && itTrace != traces.end()) {
		cout << "some traces not processed ... " << endl;
		throw 1;
	}
}

bool checkAddPointToTrace(PointTrace* trace, Point2f point, Mat descriptor) {
	ExtendedPoint* lastEp = trace->firstPoint();
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
	return(ratio < 0.990);
}

void FlowTraceProcessor::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
	cout << "frame " << frame1->index;
	frame2->keypoints.clear();
	frame2->rawKeypoints.clear();

	int delta = max(1,(int)floor(sqrt(min(frame1->image.cols, frame1->image.rows))-2));
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
			err, // indicates quality of each point?
			Size(10,10), // window size
			2); // max level

	// TODO: filter with err
	for(int i=0; i<status.size(); i++) {
		//if(err.at(i) > 30) status.at(i) = 0;
	}

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
