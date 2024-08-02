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
        bezier_drawer_draw(drawer, i, cro);
    }
}

static void add_curve(GtkButton *self, gpointer user_data) {
    (void) self;
    struct AppData *data = user_data;
    BezierDrawableCurve2D *curve = bezier_drawer_new_curve(&data->drawer);
    if (!curve) {
        return;
    }
    cairo_pattern_t *curvePattern = cairo_pattern_create_rgb(0.8, 0, 0.8);
    cairo_pattern_t *gridPattern  = cairo_pattern_create_rgb(0, 0, 0);
    bezier_curve_set_cpattern(
            curve,
            curvePattern);
    bezier_curve_set_gpattern(
            curve,
            gridPattern);
    data->drawer.isEditing = 1;
    bezier_curve_add_point(
            curve,
            data->mouse);
}

static void print_motion(
        GtkEventControllerMotion *self,
        double x,
        double y,
        gpointer data) {
    (void) self;
    struct AppData *appData = data;
    appData->mouse = (BezierPoint2D) { .posX = x, .posY = y };
    if (!appData->drawer.isEditing) {
        return;
    }
    bezier_drawer_edit_last(&appData->drawer, appData->mouse);
    gtk_widget_queue_draw(GTK_WIDGET(appData->area));
}

static GtkEventController *paint_controller(struct AppData *data) {
    GtkEventController *controller = gtk_event_controller_motion_new();
    g_signal_connect(controller, "motion", G_CALLBACK(print_motion), data);
    return controller;
}

static void add_point(BezierDrawer *drawer, double x, double y) {
    if (!drawer->isEditing) {
        return;
    }
    bezier_curve_add_point(
            bezier_drawer_curve_at(drawer, drawer->count - 1),
            (BezierPoint2D) { .posX = x, .posY = y });
}

static void end_edit(BezierDrawer *drawer) {
    drawer->isEditing = 0;
}

static void add_degree(BezierDrawer *drawer) {
    if (!drawer->isEditing) {
        return;
    }
    BezierDrawableCurve2D *curve = bezier_drawer_curve_at(drawer, drawer->count - 1);
    if (!curve || bezier_curve_add_point(curve, (BezierPoint2D) { .posX = 0, .posY = 0})) {
        return;
    }
    bezier_add_degree(curve->count - 1, curve->controls);
}

static void handle_click(
        GtkGestureClick *self,
        int n_press,
        double x,
        double y,
        struct AppData *data) {
    (void) n_press;
    guint button = gtk_gesture_single_get_current_button(GTK_GESTURE_SINGLE(self));

    if (button == GDK_BUTTON_PRIMARY) {
        add_point(&data->drawer, x, y);
        gtk_widget_queue_draw(GTK_WIDGET(data->area));
    } else if (button == GDK_BUTTON_SECONDARY) {
        end_edit(&data->drawer);
    } else if (button == GDK_BUTTON_MIDDLE) {
        add_degree(&data->drawer);
        gtk_widget_queue_draw(GTK_WIDGET(data->area));
    }
}

static GtkGesture *click_controller(struct AppData *data) {
    GtkGesture *click = gtk_gesture_click_new();
    if (!click) {
        return NULL;
    }
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(click), 0);
    g_signal_connect(click, "pressed", G_CALLBACK(handle_click), data);
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
    g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(add_curve), data);
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
        .isEditing = 0,
    };

    char *title = argc > 1 ? argv[1] : "Bezier";
    struct AppData data = {
        .drawer = drawer,
        .title = title,
        .mouse = { .posX = 0, .posY = 0 },
        .area = NULL,
    };


    g_signal_connect(app, "activate", G_CALLBACK(activate), &data);
    int exit_status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
    appdata_cleanup(&data);

    return exit_status;
}
