#ifndef _DRAWER_H
#define _DRAWER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <cairo.h>

#include "bezier.h"

typedef struct {
    BezierPoint2D *controls;
    size_t count;
    size_t capacity;
    size_t resolution;
    cairo_pattern_t *curvePattern;
    cairo_pattern_t *gridPattern;
} BezierDrawableCurve2D;

typedef struct {
    size_t count;
    size_t capacity;
    BezierDrawableCurve2D *curves;
    int isEditing;
} BezierDrawer;

int bezier_curve2d_init(BezierDrawableCurve2D *curve);

BezierDrawableCurve2D *bezier_drawer_curve_at(BezierDrawer *drawer, size_t index);

int bezier_curve_set_cpattern(BezierDrawableCurve2D *curve, cairo_pattern_t *pattern);

int bezier_curve_set_gpattern(BezierDrawableCurve2D *curve, cairo_pattern_t *pattern);

void bezier_curve_draw(BezierDrawableCurve2D *curve, cairo_t *cro);
    
int bezier_drawer_draw(BezierDrawer *drawer, size_t index, cairo_t *cro);

BezierDrawableCurve2D *bezier_drawer_new_curve(BezierDrawer *drawer);

int bezier_curve_add_point(BezierDrawableCurve2D *curve, BezierPoint2D point);

int bezier_drawer_edit_last(BezierDrawer *drawer, BezierPoint2D point);

void bezier_curve_cleanup(BezierDrawableCurve2D *curve);

void bezier_drawer_cleanup(BezierDrawer *drawer);

#ifdef __cplusplus
}
#endif

#endif
