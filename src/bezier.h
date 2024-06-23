#ifndef _BEZIER_H
#define _BEZIER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct {
    double posX;
    double posY;
} BezierPoint2D;

typedef struct {
    size_t count;
    BezierPoint2D *coeffs;
} BezierCurve;

BezierPoint2D bezier_evaluate(
        size_t count,
        const BezierPoint2D *points,
        double interpolationFactor);

BezierPoint2D *bezier_bulk(
        size_t controlPointCount,
        const BezierPoint2D *points,
        size_t resultPointCount,
        BezierPoint2D *result);

BezierCurve *bezier_compile(size_t count, const BezierPoint2D *points, BezierCurve *curve);

#ifdef __cplusplus
}
#endif

#endif
