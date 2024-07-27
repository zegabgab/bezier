#include <errno.h>

#include "drawer.h"

#define DEFAULT_RESOLUTION 100

static void draw_curve(BezierCurve2D *curve, cairo_t *cro) {
    if (curve->resolution < 2) {
        return;
    }

    cairo_new_path(cro);
    BezierPoint2D points[curve->resolution];
    bezier_bulk(curve->count, curve->controls, curve->resolution, points);
    cairo_move_to(cro, points->posX, points->posY);
    for (int i = 1; i < curve->resolution; i++) {
        cairo_line_to(cro, points[i].posX, points[i].posY);
    }
}
    
int bezier_drawer_draw(BezierDrawer *drawer, size_t index, cairo_t *cro) {
    if (index >= drawer->count) {
        return 1;
    }

    BezierCurve2D *curve = drawer->curves + index;
    draw_curve(curve, cro);
    
    return 0;
}

int bezier_drawer_new_curve(BezierDrawer *drawer) {
    if (drawer->count >= drawer->capacity) {
        BezierCurve2D *new_curves =
            realloc(drawer->curves, 2 * drawer->capacity * sizeof(BezierCurve2D));
        if (!new_curves) {
            return errno;
        }
        drawer->curves = new_curves;
        drawer->capacity *= 2;
    }

    static const unsigned int INITIAL_CAPACITY = 4;
    BezierCurve2D curve = {
        .controls = malloc(INITIAL_CAPACITY * sizeof(BezierPoint2D)),
        .count = 0,
        .capacity = INITIAL_CAPACITY,
        .resolution = DEFAULT_RESOLUTION,
    };
    if (!curve.controls) {
        return errno;
    }

    drawer->curves[drawer->count++] = curve;
    return 0;
}

int bezier_curve_add_point(BezierCurve2D *curve, BezierPoint2D point) {
    if (curve->count >= curve->capacity) {
        BezierPoint2D *new_controls =
            realloc(curve->controls, 2 * curve->capacity * sizeof(BezierPoint2D));
        if (!new_controls) {
            return errno;
        }
        curve->controls = new_controls;
        curve->capacity *= 2;
    }

    curve->controls[curve->count++] = point;
    return 0;
}

int bezier_drawer_edit_last(BezierDrawer *drawer, BezierPoint2D point) {
    if (drawer->count == 0 || drawer->curves[drawer->count - 1].count == 0) {
        return 1;
    }

    BezierCurve2D *curve = drawer->curves + drawer->count - 1;
    curve->controls[curve->count - 1] = point;
    return 0;
}

