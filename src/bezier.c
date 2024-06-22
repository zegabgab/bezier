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
