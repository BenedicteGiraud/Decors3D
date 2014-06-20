/*
 * DistanceMap.cpp
 *
 *  Created on: 2014/06/20
 *      Author: tr
 */

#include <queue>

#include "DistanceMap.h"
#include "ExtendedPoint.h"

using namespace cv;
using namespace std;

char dNeighbor[8][2] = {{0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}};
int DistanceMap::WorkingItem::nextid = 0;

DistanceMap::DistanceMap(int rows, int cols) {
	this->rows = rows;
	this->cols = cols;
}

DistanceMap::~DistanceMap() {
	// TODO Auto-generated destructor stub
}

void DistanceMap::populate(vector<ExtendedPoint*> points, double maxDistance) {
	// init intern data structure
	queue<DistanceMap::WorkingItem*> workingitems;
	DistanceMap::WorkingItem::nextid = 0;
	PixelInformationType init;
	for(int row=0; row<rows; row++) {
		for(int col=0; col<cols; col++) {
			PixelInformationType info;
			info.done = false;
			infoMat.push_back(info);
		}
	}

	// init working list
	for(ExtendedPoint* ep : points) {
		if(ep == NULL) continue;
		WorkingItem *item = new WorkingItem;
		item->col = ep->coordinates.x;
		item->row = ep->coordinates.y;
		if(item->col < 0 || item->row < 0 || item->col >= cols || item->row >= rows) {
			delete item;
			continue;
		}
		item->center.distance = 0;
		item->center.point = ep;
		item->center.coordX = ep->coordinates.x;
		item->center.coordY = ep->coordinates.y;
		workingitems.push(item);

		PixelInformationType *info = &infoMat.at(item->row * cols + item->col);
		info->nearestKeypoints.push_back(item->center);
	}

	// process working list
	while(!workingitems.empty()) {
		WorkingItem* work = workingitems.front(); workingitems.pop();
		if(work == NULL) continue;
		PixelInformationType *info = &infoMat.at(work->row *cols + work->col);
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
			if(nrow >= rows || ncol >= cols) continue;
			if(infoMat.at(nrow * cols + ncol).nearestKeypoints.size() > 3) continue;

			int distanceX = ncol-(work->center.coordX);
			int distanceY = nrow-(work->center.coordY);
			unsigned distance = distanceX*distanceX + distanceY*distanceY;
			if(distance > maxDistance) continue;

			// add new point to working list
			WorkingItem *newitem = new WorkingItem;
			newitem->col = ncol;
			newitem->row = nrow;
			newitem->center = work->center;
			newitem->center.distance = distance;
			workingitems.push(newitem);

			// actualize information
			PixelInformationType *info = &infoMat.at(nrow * cols + ncol);
			info->nearestKeypoints.push_back(newitem->center);
		}
		delete work;

		if(WorkingItem::nextid > 4*rows*cols) {
			cout << " sth is wrong ..." << endl;
			//exit(0);
			return;
		}
	}
}

ExtendedPoint* DistanceMap::getNearestKeyPoint(Point2f point) {
	// TODO
	return NULL;
}

multimap<double, ExtendedPoint*> DistanceMap::getNearestKeyPoints(Point2f point, double maxDistance) {
	multimap<double, ExtendedPoint*> result;
	int row = point.y;
	int col = point.x;
	if(row < 0 || col < 0 || row >= rows || col >= cols) return result;
	for(auto keypoint : infoMat[row * cols + col].nearestKeypoints) {
		double distance = norm(keypoint.point->coordinates - point);
		if(distance < maxDistance) {
			result.insert(pair<double, ExtendedPoint*>(distance, keypoint.point));
		}
	}
	return result;
}
