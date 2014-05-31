/*
 * Tools.h
 *
 *  Created on: 2014/05/31
 *      Author: tr
 */

#ifndef TOOLS_H_
#define TOOLS_H_

using namespace cv;

class Tools {
public:
	Tools();
	virtual ~Tools();

	inline static void verticalConcatenateMatrices(cv::Mat& input, cv::Mat& add, cv::Mat& output) {
		if(input.rows == 0) {
			output = add;
		}
		else if(add.rows == 0) {
			output = input;
		}
		else {
			vconcat(input, add, output);
		}
	}
};

#endif /* TOOLS_H_ */
