/*
 * TraceInterpolationProcessor2.h
 *
 *  Created on: 2014/06/02
 *      Author: tr
 */

#ifndef TraceInterpolationProcessor2_H_
#define TraceInterpolationProcessor2_H_

#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/flann/kdtree_index.h>

#include "processor/FrameProcessor.h"

using namespace cv;

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

class TraceInterpolationProcessor2: public FrameProcessor {
public:
	TraceInterpolationProcessor2();
	virtual ~TraceInterpolationProcessor2();

	Video* debugVideo;
	vector<cv::Mat> summedInterpolation;
	vector<cv::Mat> weight;
	vector<cv::Mat> count;
	vector<vector<PixelInformationType>> infoMats;

	virtual void processStart(Video *video);
	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image, ProcessorCallback* callback);
	virtual void processEnd(Video *video);

	cv::Mat getImage();
	/*cv::Mat getCountImage();*/

	Video getVideo();
};

#endif /* TraceInterpolationProcessor2_H_ */
