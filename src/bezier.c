#include "bezier.h"

#include <assert.h>
#include <string.h>

static inline double lerp_double(double start, double end, double factor) {
    return start + (end - start) * factor;
}

static BezierPoint2D lerp(const BezierPoint2D *start, const BezierPoint2D *end, double factor) {
    BezierPoint2D inbetween = {
        .posX = lerp_double(start->posX, end->posX, factor),
        .posY = lerp_double(start->posY, end->posY, factor),
    };
    return inbetween;
}

BezierPoint2D bezier_evaluate(
        size_t count,
        const BezierPoint2D *points,
        double interpolationFactor) {
    assert(count != 0);
    BezierPoint2D intermediatePoints[count];
    memcpy(intermediatePoints, points, sizeof intermediatePoints);
    for (size_t i = count - 1; i > 0; i--) {
        for (size_t j = 0; j < i; j++) {
            intermediatePoints[j] = 
                lerp(intermediatePoints + j, intermediatePoints + j + 1, interpolationFactor);
        }
    }
    return intermediatePoints[0];
}

BezierPoint2D *bezier_bulk(
        size_t controlPointCount,
        const BezierPoint2D *controlPoints,
        size_t resultPointCount,
        BezierPoint2D *result) {
    for (size_t i = 0; i < resultPointCount; i++) {
        result[i] = bezier_evaluate(
                controlPointCount, controlPoints, ((double) i) / (double) (resultPointCount - 1));
    }
    return result;
}

BezierCurve *bezier_compile(size_t count, const BezierPoint2D *points, BezierCurve *curve) {
    BezierPoint2D *compiledPoints = malloc(count * sizeof(BezierPoint2D));
    if (!compiledPoints) {
        return NULL;
    }
    // TODO actually compile the polynomial
    (void) points;
    free(compiledPoints);
    curve->count = 0;
    curve->coeffs = NULL;
    return curve;
}
