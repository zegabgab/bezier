#ifndef BEZIER_CONFIG_H
#define BEZIER_CONFIG_H

#include <gtk/gtk.h>

typedef struct {
    GdkRGBA curveColor;
    GdkRGBA gridColor;
} Config;

int parseConfig(const char *path, Config *config);

#endif
