#include "MovementReprojection.h"

#include "ClassKeyPointsWithNeighbor.h"
#include "entities/ExtendedPoint.h"
#include "entities/Frame.h"
#include "entities/PointTrace.h"
#include "entities/Video.h"

#include <iostream>

using namespace std;
using namespace cv;


/** AIM of the clases :
if a point is mobile, but surronded by fixed points => add it to the fix class
if a point is fix, but surrounded by mobile points => add it to the mobile class
*/


MovementReprojection::MovementReprojection(){

}

MovementReprojection::~MovementReprojection(){

}


// regarde dans la frame suivante ce qu il y a autour de ce point
void MovementReprojection::processDoubleFrame(Video* video, Frame* frame1, Frame* frame2) {
    // Choose a keypoint
    for (int keyPointNumber=0; keyPointNumber < frame1->keypoints.size() ; keyPointNumber ++){
        ExtendedPoint *keyPoint = frame1->keypoints[keyPointNumber];
        Point2f keyPoinCoord = keyPoint->coordinates;

        if(keyPoint->trace == NULL) {
            continue;
        }
        PointTrace::Type typeKeypoint = keyPoint->trace->type;
        int fix=0, mobile=0, total=0;

        // Calculate all the distance to the others keypoints
        for (int i=0; i < frame2->keypoints.size(); i++) {
            if (i == keyPointNumber) {
                continue;
            }

            ExtendedPoint *keyPoint2 = frame2->keypoints[i];
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
        double fixRatio = (double)fix/(fix+mobile);
        double mobileRatio = (double)mobile/(fix+mobile);

        // (Maybe put a thresh : more than 60% in the other class -> change the value, in the other cases no)
        // make no change if there is less than 5 points selected
        if (fix+mobile>4) {

            // in order to prevent the borders and coins to change value : thresh is not 0.5 but 0.8
            if (typeKeypoint == PointTrace::scene){

                if (fixRatio < 0.5){
               // if (mobileRatio > 0.5) {
                    keyPoint->trace->type = PointTrace::object;
                }
                else {
                    continue;
                }
            }
            /*if (typeKeypoint == PointTrace::object){
                if (fixRatio > 0.95) {
                    keyPoint->trace->type = PointTrace::scene;


            }*/
            // in the case the type is unkown, the thresh is 0.6 : add them to a categorie but
            else { // Type == unknown
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
            continue;
        }
    }
}
