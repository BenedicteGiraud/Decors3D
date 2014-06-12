#include "ClassKeyPointsWithNeighbor.h"
#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/ExtendedPoint.h"
#include "entities/PointTrace.h"
#include <iostream>

using namespace std;
using namespace cv;

ClassKeyPointsWithNeighbor::ClassKeyPointsWithNeighbor()
{
}

void ClassKeyPointsWithNeighbor::processFrame(Video* video, Frame* frame, Mat* image, ProcessorCallback* callback){


    // Choose a keypoint
    for (int keyPointNumber=0; keyPointNumber < frame->keypoints.size() ; keyPointNumber ++){
        ExtendedPoint *keyPoint = frame->keypoints[keyPointNumber];
        Point2f keyPoinCoord = keyPoint->coordinates;

        if(keyPoint->trace == NULL) {
            continue;
        }
        PointTrace::Type typeKeypoint = keyPoint->trace->type;
        int fix=0, mobile=0, total=0;

        // Calculate all the distance to the others keypoints
        for (int i=0; i < frame->keypoints.size(); i++) {
            if (i == keyPointNumber) {
                continue;
            }

            ExtendedPoint *keyPoint2 = frame->keypoints[i];
            if(keyPoint2->trace == NULL) {
                continue;
            }
            Point2f p2Coord = keyPoint2->coordinates;
            float distance = norm(keyPoinCoord - p2Coord);

            // Select the keypoints for which one the distance is inferier to THRESH : nombre de pixel d'écart
            if (distance < 50) {
                // Know the number of keypoint
                total = total+1;

                // Classified them into fix / mobile
                if (keyPoint2->trace->type == PointTrace::scene) { // scene = fix
                    fix = fix+1;
                }
                if (keyPoint2->trace->type == PointTrace::object) {
                    mobile = mobile+1;
                }
                else {
                    continue;
                }

            }
        }

        // Add the current keypoint to the good class
        double fixRatio = fix/total;
        double mobileRatio = mobile/total;

        // (Maybe put a thresh : more than 60% in the other class -> change the value, in the other cases no)
        // make no change if there is less than 5 points selected
        if (total>4) {
            // in order to prevent the borders and coins to change value : thresh is not 0.5 but 0.8
            if (typeKeypoint == PointTrace::scene){
                if (mobileRatio > 0.8) {
                    keyPoint->trace->type = PointTrace::object;
                }
                else {
                    continue;
                }
            }
            if (typeKeypoint == PointTrace::object){
                if (fixRatio > 0.8) {
                    keyPoint->trace->type = PointTrace::scene;
                }
                else {
                    continue;
                }
            }
            // in the case the type is unkown, the thresh is 0.6 : add them to a categorie but
            else { // unknown
                if (mobileRatio>0.6) {
                    keyPoint->trace->type = PointTrace::object;
                }
                if (fixRatio>0.6) {
                    keyPoint->trace->type = PointTrace::scene;
                }
                else {
                    continue;
                }
            }
        }
        else {
            cout<< "Not enough neighbors. Neighbor Number =     " << total << endl;
            continue;
        }

    }
}
