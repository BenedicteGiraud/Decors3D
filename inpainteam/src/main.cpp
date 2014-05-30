#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/features2d/features2d.hpp>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "processor/annotator/TraceAnnotator.h"
#include "processor/annotator/ResizeAnnotator.h"
#include "processor/annotator/HomographyAnnotator.h"

#include "processor/PipelineProcessor.h"
#include "processor/DoubleFrameProcessor.h"
#include "processor/OutputProcessor.h"

#include "processor/KeyPointProcessor.h"
#include "processor/KeyPointTraceProcessor.h"
#include "processor/FlowTraceProcessor.h"
#include "processor/SceneTraceClassifierProcessor.h"
#include "processor/HomographyEstimatorProcessor.h"

using namespace std;
using namespace cv;

/*
 * TODO:
 * - use optical flow to create traces
 * - hashmap frame index -> ExtendedPoint for PointTrace
 * - add high precision coordinates to points
 * - interpolate traces based on neighboring traces
 * - evaluate methods for combining traces
 *
 * - use homography estimation to improve flow trace
 * - kalman filter to make homography more stable?
 *
 *  cleanup:
 * - add delete statements to destructors
 * - comments
 * - move processor classes to subfolders (except interfaces)
 */

int main(int argc, char* argv[]) {
	if( argc < 3) {
		cout << "Missing arguments: ";
        cout << argv[0] << " [input] [output folder]\n";
		return -1;
	}
	string inputFilename = argv[1];
	string outputDirectory = argv[2];

	Video video(inputFilename);
	VideoPlayer player = video.getPlayer();

	// configure video player
	ResizeAnnotator resize;
	resize.setFactor(3);

	TraceAnnotator traceAnnotator;
	HomographyAnnotator homographyAnnotator;

	PipelineProcessor pipeline;
	pipeline.add(&traceAnnotator);
	pipeline.add(&homographyAnnotator);

	pipeline.add(&resize);
	player.setFramesAnnotator(&pipeline);
	player.setFramesPerSecond(15);

	// configure processor pipeline
	/*KeyPointProcessor keypointProcessor;
	video.applyFrameProcessor(keypointProcessor);*/

	/*KeyPointTraceProcessor keypointTraceProcessor;
	video.applyDoubleFrameProcessor(keypointTraceProcessor);*/
	FlowTraceProcessor flowTraceProcessor;
	video.applyDoubleFrameProcessor(flowTraceProcessor);

	SceneTraceClassifierProcessor SceneTraceClassifierProcessor;
	video.applyVideoProcessor(SceneTraceClassifierProcessor);

	player.play();

	HomographyEstimatorProcessor homographyEstimator;
	video.applyDoubleFrameProcessor(homographyEstimator);

	video.applyVideoProcessor(SceneTraceClassifierProcessor);

	player.play();

	// write to file
	Video annotatedOutput;
	PipelineProcessor outputPipeline;
	outputPipeline.add(&pipeline);
	OutputProcessor outputProcessor;
	outputProcessor.setOutput(&annotatedOutput);
	outputPipeline.add(&outputProcessor);

	video.applyFrameProcessor(outputPipeline);

	annotatedOutput.write(outputDirectory + "/annotatedOutput.avi");

	//video.write(outputDirectory + "/output.avi");

	return 0;
}
