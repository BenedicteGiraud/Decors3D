/*
 * CombinationVideoProvider.h
 *
 *  Created on: 2014/06/05
 *      Author: tr
 */

#ifndef COMBINATIONVIDEOPROVIDER_H_
#define COMBINATIONVIDEOPROVIDER_H_

#include <cv.h>

#include "visualization/VideoProvider.h"

class CombinationVideoProvider : public VideoProvider {
public:
	CombinationVideoProvider();
	virtual ~CombinationVideoProvider();

	class SubProvider {
	public:
		VideoProvider* provider;
		cv::Rect geometry;
	};

	int width;
	int height;
	std::vector<SubProvider> subproviders;

	void addProvider(VideoProvider *provider, int x, int y);
	void addProvider(VideoProvider *provider, cv::Rect geometry);

	virtual void start();
	virtual void finish();

	virtual bool seek(int index);
	virtual bool seekRelative(int shift);

	virtual cv::Mat getImage();
};

#endif /* COMBINATIONVIDEOPROVIDER_H_ */
