/*
 * SceneTraceClassifierProcessor.cpp
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#include <cmath>
#include <cv.h>
#include <highgui.h>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "Tools.h"

#include "SceneTraceClassifierProcessor.h"

using namespace std;

SceneTraceClassifierProcessor::SceneTraceClassifierProcessor() {

}

SceneTraceClassifierProcessor::~SceneTraceClassifierProcessor() {

}

multimap<double, PointTrace*> getHomographyErrorMap(Video* video) {
	int i = -1;
	multimap<double, PointTrace*> distances;
	for(auto trace : video->pointTraces) {
		i++;

		if(trace->points.size() >= 4) {
			//if(i > 30) break;
			double distance = 0;
			double threshold;
			ExtendedPoint* last = NULL;

			Mat homography = Mat::eye(3, 3, CV_32F);

			int homographiesIndex = 0;
			for(pair<int, ExtendedPoint*> point : trace->points) {
				if(last != NULL) {

					Mat fundamental = point.second->frame->fundamentalMatrixToLastFrame;
					if(fundamental.rows != 0) {
						Mat p1(3,1, fundamental.type());
						p1.at<double>(1,0) = last->coordinates.x;
						p1.at<double>(2,0) = last->coordinates.y;
						p1.at<double>(3,0) = 1;
						Mat p2(3,1, fundamental.type());
						p2.at<double>(1,0) = point.second->coordinates.x;
						p2.at<double>(2,0) = point.second->coordinates.y;
						p2.at<double>(3,0) = 1;

						Mat mul = p1.t() * fundamental;
						mul *= p2;
						if(std::isnan(distance) || std::isnan(norm(mul))) {
							cout << "Warning: SceneTraceClassifier got NaN value " << endl;
						}
						distance += norm(mul);
					}
					else {
						bool homographyIsValid = video->getHomography(last->frame, point.second->frame, homography);
						Point2f destPoint;
						if(homographyIsValid) {
							destPoint = Tools::applyHomography(homography,last->coordinates);
						}
						else {
							destPoint = last->coordinates;
						}
						double n = norm(destPoint - point.second->coordinates);

						if(std::isnan(distance) || std::isnan(n)) {
							cout << "Warning: SceneTraceClassifier got NaN value " << endl;
						}
						distance += n*n;
					}
				}
				last = point.second;
			}

			double normalized = distance/ trace->points.size();
			cout << "distance A " << normalized << endl;

			distances.insert(pair<double, PointTrace*>(normalized, trace));
		}
        else {
            trace->type = PointTrace::unknown;
        }
	}
	return distances;
}

void SceneTraceClassifierProcessor::process(Video* video) {
	auto distances = getHomographyErrorMap(video);
	if(distances.size() < 4) return;

	double minDist = (distances.begin()->first);
	double maxDist = (distances.rbegin()->first);
	double maxDistQuantil = maxDist;
	auto it = distances.rbegin();
	int count = 0;
	for(; it != distances.rend(); it++) {
		count ++;
		maxDistQuantil = it->first;
		if(count >= distances.size() * 0.05) {
			break;
		}
	}

	double median;
	count = 0;
	for(auto d : distances) {
		median = d.first;
		if(count >= distances.size() * 0.5) {
			break;
		}
	}

	int width = 200; int height = 100;
	Mat histogramImage(height, width, CV_8UC3, Scalar(255,255,255));
	vector<int> histogramm;
	int countMax = 0; int indexMax = 0;
	double diff = (maxDistQuantil-minDist) / width;
	for(int i=0; i<width; i++) {
		double rangeMin = minDist + i*diff;
		double rangeMax = rangeMin + diff;

		int count = distance(distances.lower_bound(rangeMin), distances.upper_bound(rangeMax));
		histogramm.push_back(count);

		if(count > countMax) {
			countMax = count;
			indexMax = i;
		}
	}
	double valueMax = minDist + indexMax*diff;

	int medianI = (median - minDist)/diff;
	for(int i=0; i<width; i++) {
		double ratio = (double)histogramm[i] / countMax;
		int lineHeight = (int)(ratio * height);
		line(histogramImage, Point(i,0), Point(i, lineHeight), Scalar(127,0,0));
	}

	double ratio = (double)histogramm[indexMax] / countMax;
	int lineHeight = (int)(ratio * height);
	line(histogramImage, Point(indexMax,0), Point(indexMax, lineHeight), Scalar(0,255,255));

	/*string windowName = "scene trace classifier histogram";
	namedWindow(windowName, WINDOW_NORMAL);
	imshow(windowName, histogramImage);
	waitKey(0);*/

	for(auto d : distances) {
		if(d.first < valueMax - 3*diff) continue;
		if(d.first < valueMax + 3*diff) {
			d.second->type = PointTrace::scene;
		}
		else if(d.first > valueMax + 10*diff) {
			d.second->type = PointTrace::object;
		}
	}
}

