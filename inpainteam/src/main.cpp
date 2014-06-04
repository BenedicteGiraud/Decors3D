#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/features2d/features2d.hpp>

#include "entities/Video.h"
#include "entities/Frame.h"
#include "processor/annotation/TraceAnnotator.h"
#include "processor/annotation/ResizeAnnotator.h"
#include "processor/annotation/HomographyAnnotator.h"

#include "processor/PipelineProcessor.h"
#include "processor/DoubleFrameProcessor.h"
#include "processor/OutputProcessor.h"

#include "processor/detection/KeyPointProcessor.h"
#include "processor/detection/KeyPointTraceProcessor.h"
#include "processor/detection/FlowTraceProcessor.h"
#include "processor/detection/SceneTraceClassifierProcessor.h"
#include "processor/detection/HomographyEstimatorProcessor.h"
#include "processor/optimization/TraceKalmanFilterProcessor.h"
#include "processor/reconstruction/TraceInterpolationProcessor.h"

using namespace std;
using namespace cv;

/*
 * TODO:
 * - interpolate result pixels based on neighboring traces
 * - evaluate methods for combining traces
 * - use fundamental matrix instead of homography
 * - use homography estimation to improve flow trace
 * - kalman filter to make homography more stable?
 *
 * optimization:
 * - hashmap frame index -> ExtendedPoint for PointTrace
 * - list of traces for frame / store as hashmap ?
 * - use k-nearest-neighbors algorithm (e.g. http://docs.opencv.org/modules/flann/doc/flann_fast_approximate_nearest_neighbor_search.html)
 *
 *  cleanup:
 * - add delete statements to destructors
 * - comments
 * - move processor classes to subfolders (except interfaces)
 */

FrameProcessor* getAnnotationProcessor() {
	PipelineProcessor* pipeline = new PipelineProcessor();

	pipeline->add(new ResizeAnnotator(7));
	pipeline->add(new TraceAnnotator());
	//pipeline->add(new HomographyAnnotator());
	return pipeline;
}

void annotateToFile(Video* video, FrameProcessor* processor, string filename) {
	Video annotatedOutput;
	PipelineProcessor outputPipeline;
	outputPipeline.add(processor);
	OutputProcessor outputProcessor;
	outputProcessor.setOutput(&annotatedOutput);
	outputPipeline.add(&outputProcessor);

	video->applyFrameProcessor(outputPipeline);

	annotatedOutput.write(filename);
}

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
	FrameProcessor* annotationProcessor = getAnnotationProcessor();
	player.setFramesAnnotator(annotationProcessor);
	player.setFramesPerSecond(15);

	// configure processor pipeline
	FlowTraceProcessor flowTraceProcessor;
	video.applyDoubleFrameProcessor(flowTraceProcessor);

	SceneTraceClassifierProcessor sceneTraceClassifierProcessor;
	video.applyVideoProcessor(sceneTraceClassifierProcessor);

	player.play();

	HomographyEstimatorProcessor homographyEstimator;
	video.applyDoubleFrameProcessor(homographyEstimator);

	TraceKalmanFilterProcessor kalmanFilter;
	video.applyDoubleFrameProcessor(kalmanFilter);

	video.applyVideoProcessor(sceneTraceClassifierProcessor);
	player.play();

	cout << "trace types "
			<< " unknown " << PointTrace::unknown
			<< " scene " << PointTrace::scene
			<< " object " << PointTrace::object << endl;
	for(PointTrace* trace : video.pointTraces) {
		cout << "trace " << trace->type << endl;
	}


	TraceInterpolationProcessor tip;
	video.applyFrameProcessor(tip);
	//player.play();

	//Mat inpaintedImg = tip.getImage();
	Video* inp = tip.debugVideo;
	//inp->play();
	VideoPlayer inpPlayer = inp->getPlayer();
	inpPlayer.setFramesAnnotator(annotationProcessor);
	inpPlayer.playWithAnnotationData(&video);

	// write to file
	annotateToFile(&video, annotationProcessor, outputDirectory + "/annotatedOutput.avi");
	/*namedWindow("test", WINDOW_NORMAL);
	imshow("test", inpaintedImg);
	waitKey();*/


	inp->write(outputDirectory + "/output.avi");

	return 0;
}
