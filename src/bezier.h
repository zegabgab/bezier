#ifndef _BEZIER_H
#define _BEZIER_H

#include <stdlib.h>

typedef struct {
    double posX;
    double posY;
} BezierPoint2D;

BezierPoint2D bezier_evaluate(
        size_t count,
        const BezierPoint2D *points,
        double interpolationFactor);

#endif
