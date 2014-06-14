#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/features2d/features2d.hpp>

#include "entities/Video.h"
#include "entities/Frame.h"

#include "visualization/VideoPlayer.h"
#include "visualization/AnnotationVideoProvider.h"
#include "visualization/CombinationVideoProvider.h"

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
#include "processor/detection/FundamentalMatrixEstimatorProcessor.h"
#include "processor/optimization/TraceKalmanFilterProcessor.h"
#include "processor/optimization/MovementReprojection.h"
#include "processor/optimization/ClassKeyPointsWithNeighbor.h"
#include "processor/reconstruction/TraceInterpolationProcessor.h"


#include "ApplicationInpainting.h"

using namespace std;
using namespace cv;

ApplicationInpainting::ApplicationInpainting()
{
}

/**
 * @brief getResizeProcessor
 * If height < 300 : give 300 to the minimal dimension and resize the sequence with this height
 * @param video
 * @return
 */
FrameProcessor* getResizeProcessor(Video *video) {
    int size = max(video->frames.front()->image.cols, video->frames.front()->image.rows);
    int destinationSize = 600;
    if(size < destinationSize) {
        return new ResizeAnnotator(((double)destinationSize)/size);
    }
    return NULL;
}

/**
 * @brief getAnnotationProcessor
 * Add informations (circle, rectangles) to the frame
 * @param video
 * @return
 */
FrameProcessor* getAnnotationProcessor(Video *video) {
    PipelineProcessor* pipeline = new PipelineProcessor();
    FrameProcessor *resize = getResizeProcessor(video);
    pipeline->add(resize);
    pipeline->add(new TraceAnnotator());
    pipeline->add(new HomographyAnnotator());
    return pipeline;
}

/**
 * @brief annotateToFile
 * Save the file frame + informations of the pipeline
 * @param video
 * @param processor
 * @param filename
 */
void annotateToFile(Video* video, FrameProcessor* processor, string filename) {
    Video annotatedOutput;
    PipelineProcessor outputPipeline;
    outputPipeline.add(processor);
    OutputProcessor outputProcessor;
    outputProcessor.setOutput(&annotatedOutput);
    outputPipeline.add(&outputProcessor);

    video->applyFrameProcessor(outputPipeline);

    annotatedOutput.write(filename, 5);
}



void ApplicationInpainting::videoTreatment(Video *video, string outputDirectory){
    // configure video player
    VideoPlayer player = video->getPlayer();

    FrameProcessor* annotationProcessor = getAnnotationProcessor(video);
    AnnotationVideoProvider annotationProvider(video, annotationProcessor);
    player.setVideoProvider(&annotationProvider);
    player.setFramesPerSecond(15);
    player.setOutputDirectory(outputDirectory);

    // configure processor pipeline
    /*FlowTraceProcessor flowTraceProcessor;
video->applyDoubleFrameProcessor(flowTraceProcessor);*/
    KeyPointProcessor keypoints;
    KeyPointTraceProcessor keypointTrace;
    video->applyFrameProcessor(keypoints);
    video->applyDoubleFrameProcessor(keypointTrace);

    SceneTraceClassifierProcessor sceneTraceClassifierProcessor;
    video->applyVideoProcessor(sceneTraceClassifierProcessor);
    MovementReprojection movementReprojection;
//    video->applyVideoProcessor(movementReprojection);
//    video->applyDoubleFrameProcessor(movementReprojection);
//    video->applyDoubleFrameProcessorInverse(movementReprojection);

    //player.play();

    /** ClassKeyPointsWithNeighbor classKeyPointsWithNeighbor;
    video->applyFrameProcessor(classKeyPointsWithNeighbor); */

    HomographyEstimatorProcessor homographyEstimator;
    FundamentalMatrixEstimatorProcessor fundamentalMatEstimator;

    for(int i=0; i<2; i++) {
		video->applyDoubleFrameProcessor(homographyEstimator);
		video->applyVideoProcessor(sceneTraceClassifierProcessor);
		//video->applyDoubleFrameProcessor(fundamentalMatEstimator);
		//video->applyVideoProcessor(sceneTraceClassifierProcessor);
    }
    
    video->applyVideoProcessor(sceneTraceClassifierProcessor);
    //player.play();

    TraceInterpolationProcessor tip;
    video->applyFrameProcessor(tip);
    //player.play();

    //Mat inpaintedImg = tip.getImage();
    Video* inp = tip.debugVideo;
    //inp->play();
    VideoPlayer inpPlayer;
    CombinationVideoProvider videoprovider;
    FrameProcessor *resize = getResizeProcessor(video);
    videoprovider.addProvider(new AnnotationVideoProvider(video, resize, NULL), 0,0);
    Mat image = videoprovider.getImage();
    int height = image.rows;
    int width = image.cols;
    videoprovider.addProvider(new AnnotationVideoProvider(video, annotationProcessor, video), width,0);
    videoprovider.addProvider(new AnnotationVideoProvider(inp, annotationProcessor, video), 0,height);
    videoprovider.addProvider(new AnnotationVideoProvider(inp, resize, NULL), width,height);
    inpPlayer.setVideoProvider(&videoprovider);
    inpPlayer.play();
    //inpPlayer.setFramesAnnotator(annotationProcessor);
    //inpPlayer.playWithAnnotationData(video);

    // write to file
    annotateToFile(video, annotationProcessor, outputDirectory + "/annotatedvideo->avi");
    annotateToFile(inp, annotationProcessor, outputDirectory + "/annotatedInpainting.avi");
    inp->write(outputDirectory + "/inpainting.avi", 5);
}
