#ifndef ApplicationInpaintingThomas_H
#define ApplicationInpaintingThomas_H

#include "entities/Video.h"

#include "processor/detection/CannyFlowTrace.h"


class ApplicationInpaintingThomas
{
public:
    ApplicationInpaintingThomas();

    static void videoTreatment(Video *video, std::string outputDirectory);

};

#endif // ApplicationInpaintingThomas_H
