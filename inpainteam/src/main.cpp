#include <iostream>

#include "entities/Video.h"
#include "ApplicationInpainting.h"
#include "ApplicationInpaintingThomas.h"
#include "Segmentation.h"

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
 * - use k-nearest-neighbors algorithm (e.g. http://docs.opencv.org/modules/flann/doc/
 * flann_fast_approximate_nearest_neighbor_search.html)
 *
 *  cleanup:
 * - add delete statements to destructors
 * - comments
 * - move processor classes to subfolders (except interfaces)
 */


/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
	// Check that the number of arguments is good or specify how many the user have to enter
	if( argc != 3) {
		cout << "Invalid arguments: ";
		cout << argv[0] << " [input] [output folder]\n";
		return -1;
	}
	string inputFilename = argv[1];
	string outputDirectory = argv[2];

	VideoPlayer player;
	player.setOutputDirectory(outputDirectory);

	/*Segmentation seg(inputFilename, 860);
	vector<Video*> listOfSequence;
	Video* nextSeq;
	while( (nextSeq = seg.nextSequence()) != NULL) {
		player.setVideo(nextSeq);
		player.play();
		listOfSequence.push_back(nextSeq);
	}*/

	Video video(inputFilename);
    ApplicationInpaintingThomas::videoTreatment(&video, outputDirectory);

	return 0;
}
