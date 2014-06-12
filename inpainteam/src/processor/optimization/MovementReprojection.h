#ifndef MOVEMENTREPROJECTION_H
#define MOVEMENTREPROJECTION_H

#include "processor/DoubleFrameProcessor.h"


class MovementReprojection : public DoubleFrameProcessor
{
public:
    MovementReprojection();
    virtual ~MovementReprojection();

    virtual void processDoubleFrame(Video* video, Frame* frame1, Frame* frame2);

};

#endif // MOVEMENTREPROJECTION_H
