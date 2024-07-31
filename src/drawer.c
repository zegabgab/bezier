#include <errno.h>

#include "drawer.h"
#include "arrays.h"

#define DEFAULT_RESOLUTION 100

static cairo_pattern_t *default_pattern() {
    return cairo_pattern_create_rgb(0, 0, 0);
}

int bezier_curve2d_init(BezierDrawableCurve2D *curve) {
    if (!curve) {
        return 1;
    }
    *curve = (BezierDrawableCurve2D) {
        .controls = NULL,
        .count = 0,
        .capacity = 0,
        .resolution = DEFAULT_RESOLUTION,
        .curvePattern = NULL,
        .gridPattern = NULL,
    };
    return 0;
}

BezierDrawableCurve2D *bezier_drawer_curve_at(BezierDrawer *drawer, size_t index) {
    return index < drawer->count ? drawer->curves + index : NULL;
}

int bezier_curve_set_cpattern(BezierDrawableCurve2D *curve, cairo_pattern_t *pattern) {
    if (!curve) {
        return 1;
    }
    curve->curvePattern = pattern;
    return 0;
}

int bezier_curve_set_gpattern(BezierDrawableCurve2D *curve, cairo_pattern_t *pattern) {
    if (!curve) {
        return 1;
    }
    curve->gridPattern = pattern;
    return 0;
}

static void draw_points(size_t count, BezierPoint2D *points, cairo_t *cro) {
    cairo_new_path(cro);
    cairo_move_to(cro, points->posX, points->posY);
    for (size_t i = 1; i < count; i++) {
        cairo_line_to(cro, points[i].posX, points[i].posY);
    }
    cairo_stroke(cro);
}

void bezier_curve_draw(BezierDrawableCurve2D *curve, cairo_t *cro) {
    if (curve->resolution < 2) {
        return;
    }

    BezierPoint2D points[curve->resolution];
    bezier_bulk(curve->count, curve->controls, curve->resolution, points);
    if (curve->gridPattern != NULL) {
        cairo_set_source(cro, curve->gridPattern);
        draw_points(curve->count, curve->controls, cro);
    }
    if (curve->curvePattern != NULL) {
        cairo_set_source(cro, curve->curvePattern);
        draw_points(curve->resolution, points, cro);
    }
}
    
int bezier_drawer_draw(BezierDrawer *drawer, size_t index, cairo_t *cro) {
    BezierDrawableCurve2D *curve = bezier_drawer_curve_at(drawer, index);
    if (!curve) {
        return 1;
    }

    bezier_curve_draw(curve, cro);
    return 0;
}

BezierDrawableCurve2D *bezier_drawer_new_curve(BezierDrawer *drawer) {
    if (drawer->count >= drawer->capacity) {
        int error;
        DOUBLE_CAPACITY(drawer->curves, drawer->capacity, error);
        if (error) {
            return NULL;
        }
    }

    BezierDrawableCurve2D curve = {
        .controls = NULL,
        .count = 0,
        .capacity = 0,
        .resolution = DEFAULT_RESOLUTION,
    };

    size_t index = drawer->count;
    drawer->curves[drawer->count++] = curve;
    return bezier_drawer_curve_at(drawer, index);
}

int bezier_curve_add_point(BezierDrawableCurve2D *curve, BezierPoint2D point) {
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

int bezier_drawer_edit_last(BezierDrawer *drawer, BezierPoint2D point) {
    if (drawer->count == 0 || drawer->curves[drawer->count - 1].count == 0) {
        return 1;
    }

    BezierDrawableCurve2D *curve = drawer->curves + drawer->count - 1;
    curve->controls[curve->count - 1] = point;
    return 0;
}

void bezier_curve_cleanup(BezierDrawableCurve2D *curve) {
    free(curve->controls);
    curve->count = 0;
    curve->capacity = 0;
    curve->controls = NULL;
    cairo_pattern_destroy(curve->curvePattern);
    curve->curvePattern = NULL;
    cairo_pattern_destroy(curve->gridPattern);
}

void bezier_drawer_cleanup(BezierDrawer *drawer) {
    for (size_t i = 0; i < drawer->count; i++) {
        bezier_curve_cleanup(drawer->curves + i);
    }
    free(drawer->curves);
    drawer->count = 0;
    drawer->capacity = 0;
    drawer->curves = NULL;
}
