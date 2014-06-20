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
#include "processor/reconstruction/TraceInterpolationProcessor2.h"


#include "ApplicationInpaintingThomas.h"

using namespace std;
using namespace cv;

ApplicationInpaintingThomas::ApplicationInpaintingThomas()
{
}

extern FrameProcessor* getResizeProcessor(Video *video);
extern FrameProcessor* getAnnotationProcessor(Video *video);
extern void annotateToFile(Video* video, FrameProcessor* processor, string filename);

void ApplicationInpaintingThomas::videoTreatment(Video *video, string outputDirectory){
    // configure video player
    VideoPlayer player = video->getPlayer();
    FrameProcessor* annotationProcessor = getAnnotationProcessor(video);
    AnnotationVideoProvider annotationProvider(video, annotationProcessor);
    player.setVideoProvider(&annotationProvider);
    player.setFramesPerSecond(15);
    player.setOutputDirectory(outputDirectory);

    // configure processor pipeline
    /*FlowTraceProcessor flowTraceProcessor;
    video->applyDoubleFrameProcessor(flowTraceProcessor); //*/

    KeyPointProcessor keypoints;
    KeyPointTraceProcessor keypointTrace;
    video->applyFrameProcessor(keypoints);
    video->applyDoubleFrameProcessor(keypointTrace); //*/

    //player.play();

    SceneTraceClassifierProcessor sceneTraceClassifierProcessor;
    //MovementReprojection movementReprojection;
    //video->applyVideoProcessor(movementReprojection);

    //player.play();

    /*ClassKeyPointsWithNeighbor classKeyPointsWithNeighbor;
    video->applyFrameProcessor(classKeyPointsWithNeighbor);*/
    
    HomographyEstimatorProcessor homographyEstimator;
    FundamentalMatrixEstimatorProcessor fundamentalMatEstimator;

    for(int i=0; i<1; i++) {
		video->applyDoubleFrameProcessor(homographyEstimator);
		//video->applyDoubleFrameProcessor(keypointTrace);
		video->applyVideoProcessor(sceneTraceClassifierProcessor);
		//video->applyDoubleFrameProcessor(fundamentalMatEstimator);
		//video->applyVideoProcessor(sceneTraceClassifierProcessor);
    }
    
    video->applyVideoProcessor(sceneTraceClassifierProcessor);
    player.play();

    TraceInterpolationProcessor2 tip;
    video->applyFrameProcessor(tip);
    //player.play();

    //Mat inpaintedImg = tip.getImage();
    Video* inp = tip.debugVideo;
    Video inp2 = tip.getVideo();
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
    videoprovider.addProvider(new AnnotationVideoProvider(&inp2, annotationProcessor, video), 0,2*height);
    videoprovider.addProvider(new AnnotationVideoProvider(&inp2, resize, NULL), width,2*height);
    inpPlayer.setVideoProvider(&videoprovider);
    inpPlayer.setOutputDirectory(outputDirectory);
    inpPlayer.play();
    //inpPlayer.setFramesAnnotator(annotationProcessor);
    //inpPlayer.playWithAnnotationData(video);

    // write to file
    /*annotateToFile(video, annotationProcessor, outputDirectory + "/annotatedvideo->avi");
    annotateToFile(inp, annotationProcessor, outputDirectory + "/annotatedInpainting.avi");
    inp->write(outputDirectory + "/inpainting.avi", 5); //*/
}
