#include <stdio.h>
#include <gtk/gtk.h>

#include "bezier.h"

static void draw_points(cairo_t *cro, size_t count, BezierPoint2D *points) {
    if (count <= 1) {
        return;
    }

    cairo_new_path(cro);
    cairo_move_to(cro, points->posX, points->posY);
    points++;

    for (int i = 1; i < count; i++, points++) {
        cairo_line_to(cro, points->posX, points->posY);
    }

    cairo_stroke(cro);
}

static void draw_curve(
        cairo_t *cro,
        size_t count,
        BezierPoint2D *controls,
        size_t resolution) {
    if (count <= 1) {
        return;
    }

    BezierPoint2D results[resolution];
    bezier_bulk(count, controls, resolution, results);
    draw_points(cro, resolution, results);
}

static void draw_function(
        GtkDrawingArea *area,
        cairo_t *cro,
        int width,
        int height,
        gpointer data) {
    (void) data;
    GdkRGBA color;
    static const int CONTROLS = 4;
    BezierPoint2D controlPoints[CONTROLS];
    static const int RESOLUTION = 50;
    BezierPoint2D results[RESOLUTION];

    controlPoints[0].posX = 50;
    controlPoints[0].posY = 50;
    controlPoints[1].posX = 0;
    controlPoints[1].posY = 0;
    controlPoints[2].posX = 100;
    controlPoints[2].posY = 0;
    controlPoints[3].posX = 200;
    controlPoints[3].posY = 100;

    draw_curve(cro, CONTROLS, controlPoints, 50);
}

static void print_hello(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    g_print("helloooo\n");
}

static void add_area(GtkBox *box) {
    GtkWidget *area = gtk_drawing_area_new();
    if (!area) {
        fprintf(stderr, "Drawing area could not be created\n");
        return;
    }
    static const int AREA_SIZE = 1000;
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(area), AREA_SIZE);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(area), AREA_SIZE);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_function, NULL, NULL);
    gtk_box_append(box, area);
}

static GtkWidget *button_box(GtkWindow *window) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    if (!box) {
        fprintf(stderr, "Box could not be created\n");
        return box;
    }

    GtkWidget *button = gtk_button_new_with_label("this is a hello");
    if (!button) {
        fprintf(stderr, "Button could not be created\n");
        return box;
    }
    gtk_box_append(GTK_BOX(box), button);

    GtkWidget *quit = gtk_button_new_with_label("this is a quit");
    if (!quit) {
        fprintf(stderr, "Quit button could not be created\n");
        return box;
    }
    gtk_box_append(GTK_BOX(box), quit);

    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    g_signal_connect_swapped(quit, "clicked", G_CALLBACK(gtk_window_destroy), window);

    gtk_widget_set_halign(box, GTK_ALIGN_START);
    gtk_widget_set_valign(box, GTK_ALIGN_START);

    return box;
}

static void apply(GtkApplication *app, gpointer data) {
    char *title = (char*) data;
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
        fprintf(stderr, "Window could not be created\n");
        return;
    }

    GtkWidget *buttons = button_box(GTK_WINDOW(window));
    if (!buttons) {
        fprintf(stderr, "Buttons could not be created\n");
        return;
    }

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_box_append(GTK_BOX(box), buttons);
    gtk_window_set_child(GTK_WINDOW(window), box);
    
    const int WINDOW_WIDTH = 500;
    const int WINDOW_HEIGHT = 300;
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_title(GTK_WINDOW(window), title);

    add_area(GTK_BOX(box));

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    printf("Starting %s\n", argv[0]);
    GtkApplication *app = gtk_application_new("gabel.bezier", G_APPLICATION_FLAGS_NONE);
    if (!app) {
        fprintf(stderr, "Application could not be created\n");
        return 1;
    }
    char *title = argc > 1 ? argv[1] : "Bezier";
    g_signal_connect(app, "activate", G_CALLBACK(apply), title);
    int exit_status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    return exit_status;
}
