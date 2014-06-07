#ifndef SEGMENTATION_H
#define SEGMENTATION_H
#include <string>
#include "entities/Video.h"
#include <cv.h>
#include <highgui.h>

class Segmentation
{
public:

    float histRatio;
    VideoCapture movie;

    // Number of bins for the 3D HSV histogram
    int hSize = 8, sSize = 4, vSize = 4;
    int sizes[3] = { hSize, sSize, vSize };

    //the current frame
    Mat frame;

    Mat nextFrame;

    //the current frame in HSV
    Mat hsvframe;

    //The current histogram
    Mat hist;

    //The next histogram
    Mat nextHist;

    Segmentation(std::string filename,
                double seconde_ = 0.,
                float histRatio_ = 0.75);

    Video *nextSequence();

    bool goToNextFrame();

};

#endif // SEGMENTATION_H
