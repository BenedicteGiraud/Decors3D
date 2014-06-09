#include "ClassKeyPointsWithNeighbor.h"
#include "entities/Video.h"
#include "entities/Frame.h"
#include "entities/ExtendedPoint.h"

ClassKeyPointsWithNeighbor::ClassKeyPointsWithNeighbor()
{
}

void ClassKeyPointsWithNeighbor::processFrame(Video* video, Frame* frame, Mat* image, ProcessorCallback* callback){


    // Choose a keypoint
    for (int keyPointNumber=0; keyPointNumber < frame->keypoints.size() ; keyPointNumber ++){
        Point2f keyPoinCoord = frame->keypoints[keyPointNumber]->coordinates;


        int fix=0, mobile=0, total=0;

        // Calculate all the distance to the others keypoints
        for (int i=0; i < frame->keypoints.size(); i++) {
            if (i == keyPointNumber) {
                continue;
            }
            Point2f p2Coord = frame->keypoints[i]->coordinates;
            float distance = norm(keyPoinCoord - p2Coord);

            // Select the keypoints for which one the distance is inferier to THRESH
            if (distance < thresh) {
                // Know the number of keypoint
                total = total+1;

                // Classified them into fix / mobile
                if (frame->keypoints[i]->type == scene) { // scene = fix
                    fix = fix+1;
                }
                if (frame->keypoints[i]->type == object) {
                    mobile = mobile+1;
                }
                else {
                    continue;
                }

            }
        }



        // Add the current keypoint to the good class
        // (Maybe put a thresh : more than 60% in the other class -> change the value, in the other cases no)

    }
}
