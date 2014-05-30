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

#include "processor/KeyPointProcessor.h"
#include "processor/KeyPointTraceProcessor.h"
#include "processor/SceneTraceClassifierProcessor.h"
#include "processor/HomographyEstimatorProcessor.h"

using namespace std;
using namespace cv;

/*
 * TODO:
 * - use optical flow to create traces
 * - add high precision coordinates to points
 * - interpolate traces based on neighboring traces
 * - evaluate methods for combining traces
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

	KeyPointProcessor keypointProcessor;
	video.applyFrameProcessor(keypointProcessor);

	KeyPointTraceProcessor keypointTraceProcessor;
	video.applyDoubleFrameProcessor(keypointTraceProcessor);

	SceneTraceClassifierProcessor SceneTraceClassifierProcessor;
	video.applyVideoProcessor(SceneTraceClassifierProcessor);

	HomographyEstimatorProcessor homographyEstimator;
	video.applyDoubleFrameProcessor(homographyEstimator);

	video.applyVideoProcessor(SceneTraceClassifierProcessor);

	ResizeAnnotator resize;
	resize.setFactor(3);

	TraceAnnotator traceAnnotator;
	HomographyAnnotator homographyAnnotator;

	PipelineProcessor pipeline;
	pipeline.add(&traceAnnotator);
	pipeline.add(&homographyAnnotator);

	pipeline.add(&resize);
	player.setFramesAnnotator(&pipeline);
	player.setFramesPerSecond(5);
	player.play();

	//video.write(outputDirectory + "/output.avi");

	return 0;
}
