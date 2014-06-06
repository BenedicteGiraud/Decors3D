#ifndef APPLICATIONINPAINTING_H
#define APPLICATIONINPAINTING_H
#include "entities/Video.h"



class ApplicationInpainting
{
public:
    ApplicationInpainting();

    static void videoTreatment(Video *video, std::string outputDirectory);

};

#endif // APPLICATIONINPAINTING_H
