#include <stdio.h>
#include <gtk/gtk.h>

#include "bezier.h"
#include "drawer.h"

struct AppData {
    BezierDrawer drawer;
    char *title;
    BezierPoint2D mouse;
    GtkDrawingArea *area;
};

static void appdata_cleanup(struct AppData *data) {
    bezier_drawer_cleanup(&data->drawer);
}

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
    (void) area;
    struct AppData *appData = data;
    BezierDrawer *drawer = &appData->drawer;

    for (int i = 0; i < drawer->count; i++) {
        BezierDrawableCurve2D *curve = drawer->curves + i;
        cairo_set_source_rgb(cro, 0, 0, 0);
        draw_points(cro, curve->count, curve->controls);
        cairo_set_source_rgb(cro, 0.8, 0., 0.8);
        draw_curve(cro, curve->count, curve->controls, curve->resolution);
    }
}

static void print_motion(
        GtkEventControllerMotion *self,
        double x,
        double y,
        gpointer data) {
    (void) self;
    struct AppData *appData = data;
    appData->mouse = (BezierPoint2D) { .posX = x, .posY = y };
    bezier_drawer_edit_last(&appData->drawer, appData->mouse);
    gtk_widget_queue_draw(GTK_WIDGET(appData->area));
}

static GtkEventController *paint_controller(struct AppData *data) {
    GtkEventController *controller = gtk_event_controller_motion_new();
    g_signal_connect(controller, "motion", G_CALLBACK(print_motion), data);
    return controller;
}

static void add_point(
        GtkGestureClick *self,
        int n_press,
        double x,
        double y,
        struct AppData *data) {
    (void) self;
    (void) n_press;
    data->mouse = (BezierPoint2D) { .posX = x, .posY = y };
    bezier_curve_add_point(data->drawer.curves + data->drawer.count - 1, data->mouse);
    gtk_widget_queue_draw(GTK_WIDGET(data->area));
}

static GtkGesture *click_controller(struct AppData *data) {
    GtkGesture *click = gtk_gesture_click_new();
    if (!click) {
        return NULL;
    }
    g_signal_connect(click, "pressed", G_CALLBACK(add_point), data);
    return click;
}

static void add_area(GtkBox *box, struct AppData *data) {
    GtkWidget *area = gtk_drawing_area_new();
    if (!area) {
        fprintf(stderr, "Drawing area could not be created\n");
        return;
    }
    data->area = GTK_DRAWING_AREA(area);
    static const int AREA_SIZE = 1000;
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(area), AREA_SIZE);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(area), AREA_SIZE);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_function, data, NULL);
    gtk_box_append(box, area);

    GtkEventController *painter = paint_controller(data);
    gtk_widget_add_controller(area, painter);
    GtkGesture *click = click_controller(data);
    gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER(click));
}

static GtkWidget *new_curve_button(struct AppData *data) {
    (void) data;
    GtkWidget *button = gtk_button_new_with_label("New curve");
    if (!button) {
        return NULL;
    }
    return button;
}

static GtkWidget *button_box(GtkWindow *window, struct AppData *data) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    if (!box) {
        return box;
    }

    GtkWidget *button = new_curve_button(data);
    if (!button) {
        return box;
    }
    gtk_box_append(GTK_BOX(box), button);

    GtkWidget *quit = gtk_button_new_with_label("Quit");
    if (!quit) {
        return box;
    }
    gtk_box_append(GTK_BOX(box), quit);

    g_signal_connect_swapped(quit, "clicked", G_CALLBACK(gtk_window_destroy), window);

    gtk_widget_set_halign(box, GTK_ALIGN_START);
    gtk_widget_set_valign(box, GTK_ALIGN_START);

    return box;
}

static void activate(GtkApplication *app, gpointer data) {
    struct AppData *appData = data;
    char *title = appData->title;
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
        return;
    }

    GtkWidget *buttons = button_box(GTK_WINDOW(window), appData);
    if (!buttons) {
        return;
    }

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_box_append(GTK_BOX(box), buttons);
    gtk_window_set_child(GTK_WINDOW(window), box);
    
    const int WINDOW_WIDTH = 500;
    const int WINDOW_HEIGHT = 300;
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_title(GTK_WINDOW(window), title);

    add_area(GTK_BOX(box), appData);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("gabel.bezier", G_APPLICATION_FLAGS_NONE);
    if (!app) {
        return 1;
    }

    BezierDrawer drawer = {
        .curves = NULL,
        .count = 0,
        .capacity = 0,
    };

    char *title = argc > 1 ? argv[1] : "Bezier";
    struct AppData data = {
        .drawer = drawer,
        .title = title,
        .mouse = { .posX = 0, .posY = 0 },
        .area = NULL,
    };

    bezier_drawer_new_curve(&data.drawer);
    bezier_curve_add_point(data.drawer.curves, data.mouse);

    g_signal_connect(app, "activate", G_CALLBACK(activate), &data);
    int exit_status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
    appdata_cleanup(&data);

    return exit_status;
}
