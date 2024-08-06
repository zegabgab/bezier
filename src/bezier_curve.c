#include "bezier_curve.h"
#include "arrays.h"

BezierPoint2D bezier_curve_evaluate(const BezierCurve2D *curve, double weight) {
    return bezier_evaluate(curve->count, curve->controls, weight);
}

int bezier_curve_evaluate_safe(const BezierCurve2D *curve, double weight, BezierPoint2D *result) {
    if (curve == NULL) {
        return 1;
    }
    
    *result = bezier_curve_evaluate(curve, weight);
    return 0;
}

int bezier_curve_bulk(const BezierCurve2D *curve, size_t count, BezierPoint2D *buffer) {
    return bezier_bulk(curve->count, curve->controls, count, buffer) == NULL;
}

int bezier_curve_bulk_safe(const BezierCurve2D *curve, size_t count, BezierPoint2D *buffer) {
    return curve == NULL || buffer == NULL ? 1
        : bezier_curve_bulk(curve, count, buffer);
}

BezierPoint2D bezier_curve_control(const BezierCurve2D *curve, ssize_t index) {
    return (curve->controls + (index >= 0 ? 0 : curve->count))[index];
}

int bezier_curve_control_safe(const BezierCurve2D *curve, ssize_t index, BezierPoint2D *point) {
    if (curve == NULL || index >= curve->count || -index > curve->count) {
        return 1;
    }
    
    *point = bezier_curve_control(curve, index);
    return 0;
}

int bezier_curve_append(BezierCurve2D *curve, BezierPoint2D point) {
    if (curve->count >= curve->capacity) {
        int error;
        DOUBLE_CAPACITY(curve->controls, curve->capacity, error);
        if (error) {
            return error;
        }
    }

    curve->controls[curve->count++] = point;
    return 0;
}

int bezier_curve_append_safe(BezierCurve2D *curve, BezierPoint2D point) {
    return curve == NULL ? 1 : bezier_curve_append(curve, point);
}

int bezier_curve_append_coords(BezierCurve2D *curve, double x, double y) {
    return bezier_curve_append(curve, (BezierPoint2D) { .posX = x, .posY = y });
}

int bezier_curve_append_coords_safe(BezierCurve2D *curve, double x, double y) {
    return curve == NULL ? 1 : bezier_curve_append_coords(curve, x, y);
}
