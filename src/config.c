#include <stdio.h>

#include "config.h"

static int scanColor(FILE *file, GdkRGBA *color) {
    int scanned = fscanf(file, " { %f , %f , %f } ",
            &color->red,
            &color->green,
            &color->blue);
    printf("Scanned items: %d\n", scanned);
    return scanned != 3;
}

static int configFromFile(FILE *file, Config *config) {
    fscanf(file, "curve =");
    int curve = scanColor(file, &config->curveColor);
    config->curveColor.alpha = 0;
    fscanf(file, "grid =");
    int grid = scanColor(file, &config->gridColor);
    config->gridColor.alpha = 0;
    return curve || grid;
}

int parseConfig(const char *path, Config *config) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return 1;
    }

    int result = configFromFile(file, config);

    fclose(file);
    return result;
}
