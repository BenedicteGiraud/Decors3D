#ifndef MOVEMENTREPROJECTION_H
#define MOVEMENTREPROJECTION_H

#include "processor/VideoProcessor.h"

class MovementReprojection : public VideoProcessor
{
public:
    MovementReprojection();
    virtual ~MovementReprojection();

    virtual void process(Video * video);
};

#endif // MOVEMENTREPROJECTION_H
