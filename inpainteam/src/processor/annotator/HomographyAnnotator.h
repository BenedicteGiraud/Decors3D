/*
 * HomographyAnnotator.h
 *
 *  Created on: 2014/05/28
 *      Author: tr
 */

#ifndef HomographyAnnotator_H_
#define HomographyAnnotator_H_

#include "processor/FrameProcessor.h"

class HomographyAnnotator : public FrameProcessor {
public:
	HomographyAnnotator();
	virtual ~HomographyAnnotator();

	std::vector<cv::Point2f> points;

	virtual void processFrame(Video* video, Frame* frame, cv::Mat* image);
};

#endif /* HomographyAnnotator_H_ */
