/*
 * CombinationVideoProvider.cpp
 *
 *  Created on: 2014/06/05
 *      Author: tr
 */

#include <cv.h>

#include "CombinationVideoProvider.h"

using namespace cv;
using namespace std;

CombinationVideoProvider::CombinationVideoProvider() {
	width = 0;
	height = 0;
}

CombinationVideoProvider::~CombinationVideoProvider() {

}

void CombinationVideoProvider::addProvider(VideoProvider *provider, int x, int y) {
	Rect geometry(x, y, provider->getImage().cols, provider->getImage().rows);
	addProvider(provider, geometry);
}

void CombinationVideoProvider::addProvider(VideoProvider *provider, Rect geometry) {
	SubProvider subprovider;
	subprovider.provider = provider;
	subprovider.geometry = geometry;
	if(geometry.x + geometry.width > width) width = geometry.x + geometry.width;
	if(geometry.y + geometry.height > height) height = geometry.y + geometry.height;
	subproviders.push_back(subprovider);
}

void CombinationVideoProvider::start() {
	for(SubProvider subprovider : subproviders) {
		subprovider.provider->start();
	}
}

void CombinationVideoProvider::finish() {
	for(SubProvider subprovider : subproviders) {
		subprovider.provider->start();
	}
}

bool CombinationVideoProvider::seek(int index) {
	bool successful = true;
	for(SubProvider subprovider : subproviders) {
		successful &= subprovider.provider->seek(index);
	}
	return successful;
}

bool CombinationVideoProvider::seekRelative(int shift) {
	bool successful = true;
	for(SubProvider subprovider : subproviders) {
		successful &= subprovider.provider->seekRelative(shift);
	}
	return successful;
}

Mat CombinationVideoProvider::getImage() {
	Mat output(height,width, CV_8UC3, Scalar(0,0,0));

	for(SubProvider subprovider : subproviders) {
		Mat suboutput = subprovider.provider->getImage();
		cout << "suboutput size " << suboutput.cols << "x" << suboutput.rows << endl;
		cout << "geometry size " << subprovider.geometry << endl;
		cout << "output size " << output.cols << "x" << output.rows << endl;
		suboutput.copyTo(output(subprovider.geometry));
	}

	return output;
}
