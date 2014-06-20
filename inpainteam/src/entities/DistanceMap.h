/*
 * DistanceMap.h
 *
 *  Created on: 2014/06/20
 *      Author: tr
 */

#ifndef DISTANCEMAP_H_
#define DISTANCEMAP_H_

#include <cv.h>

class ExtendedPoint;

class DistanceMap {
private:
	int rows;
	int cols;

public:
	DistanceMap(int rows, int cols);
	virtual ~DistanceMap();

	struct DistanceType {
		unsigned int distance;
		int coordX;
		int coordY;
		ExtendedPoint* point;
	};
	struct PixelInformationType {
		bool done;
		std::vector<DistanceType> nearestKeypoints;
	};
	struct WorkingItem {
		static int nextid;
		int id = nextid++;
		int col;
		int row;
		//PointTrace* trace;
		DistanceType center;
	} ;

	std::vector<PixelInformationType> infoMat;

	void populate(std::vector<ExtendedPoint*> points, double maxDistance);
	ExtendedPoint* getNearestKeyPoint(cv::Point2f point);
	std::multimap<double, ExtendedPoint*> getNearestKeyPoints(cv::Point2f point, double maxDistance);
};

#endif /* DISTANCEMAP_H_ */
