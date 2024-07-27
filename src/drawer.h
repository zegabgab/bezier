#ifndef _DRAWER_H
#define _DRAWER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "bezier.h"

typedef struct {
    BezierPoint2D *controls;
    size_t count;
} BezierCurve2D;

typedef struct {
    int isActive;
    size_t count;
    BezierCurve2D *curves;
} Drawer;
    

#ifdef __cplusplus
}
#endif

#endif
