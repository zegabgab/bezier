#ifndef _BEZIER_CURVE_H
#define _BEZIER_CURVE_H

#include "bezier.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A control point representation of a bézier curve,
 * with the capability to append control points.
 *
 * Invariants:
 *  - count <= capacity
 *  - for all i < count: controls[i] is a valid BezierPoint2D
 */
typedef struct {
    size_t count;
    size_t capacity;
    BezierPoint2D *controls;
} BezierCurve2D;

/**
 * Initializes a BezierCurve2D with zero control points
 */
#define BEZIER_CURVE2D_INIT(curve) curve = (BezierCurve2D) {    \
        .count = 0,                                             \
        .capacity = 0,                                          \
        .controls = NULL,                                       \
        }

/**
 * Finalizes a BezierCurve2D
 */
#define BEZIER_CURVE2D_FIN(curve)                               \
    free((curve).controls);                                     \
    BEZIER_CURVE2D_INIT(curve);

/**
 * Evaluates curve at weight.
 *
 * @param[in] curve  The curve to be evaluated
 * @param[in] weight The parameter at which to evaluate the curve, usually between 0 and 1
 * @pre curve != NULL
 */
BezierPoint2D bezier_curve_evaluate(const BezierCurve2D *curve, double weight);

/**
 * Evaluates curve at [count] evenly spaced weights, starting with 0 and ending with 1,
 * and places the results in [buffer].
 *
 * @param[in]  curve  The curve to be evaluated
 * @param[in]  count  The number of points to be calculated
 * @param[out] buffer The array where the results of the evaluations are stored
 * @pre curve != NULL && buffer != NULL
 */
int bezier_curve_bulk(const BezierCurve2D *curve, size_t count, BezierPoint2D *buffer);

/**
 * Returns the indexth control point of curve. Counts backwards if index is negative.
 *
 * @param[in] curve The curve whose control point is sought after
 * @param[in] index The index of the wanted control point
 * @pre (index >= 0 && index < curve->count) || (index < 0 && -index <= curve->count)
 */
BezierPoint2D bezier_curve_control(const BezierCurve2D *curve, ssize_t index);

/**
 * Appends the given point to the end of curve.
 *
 * @pre curve != NULL
 */
int bezier_curve_append(BezierCurve2D *curve, BezierPoint2D point);

/**
 * Appends the point (x, y) to the end of curve.
 *
 * @pre curve != NULL
 */
int bezier_curve_append_coords(BezierCurve2D *curve, double x, double y);

#ifdef __cplusplus
}
#endif

#endif
