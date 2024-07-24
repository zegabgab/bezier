#include <stdio.h>
#include <gtk/gtk.h>

static void print_hello(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    g_print("helloooo\n");
}

static void apply(GtkApplication *app, gpointer data) {
    char *title = (char*) data;
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
        fprintf(stderr, "Window could not be created\n");
        return;
    }
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    if (!box) {
        fprintf(stderr, "Box could not be created\n");
        return;
    }

    gtk_window_set_child(GTK_WINDOW(window), box);

    GtkWidget *button = gtk_button_new_with_label("this is a hello");
    if (!button) {
        fprintf(stderr, "Button could not be created\n");
        return;
    }
    gtk_box_append(GTK_BOX(box), button);

    GtkWidget *quit = gtk_button_new_with_label("this is a quit");
    if (!quit) {
        fprintf(stderr, "Quit button could not be created\n");
        return;
    }
    gtk_box_append(GTK_BOX(box), quit);

    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    g_signal_connect_swapped(quit, "clicked", G_CALLBACK(gtk_window_destroy), window);

    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    gtk_window_set_title(GTK_WINDOW(window), title);
    const int WINDOW_WIDTH = 500;
    const int WINDOW_HEIGHT = 300;
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    printf("Starting %s\n", argv[0]);
    GtkApplication *app = gtk_application_new("gabel.bezier", G_APPLICATION_FLAGS_NONE);
    if (!app) {
        fprintf(stderr, "Application could not be created\n");
        return 1;
    }
    char *title = argc > 1 ? argv[1] : "Default title";
    g_signal_connect(app, "activate", G_CALLBACK(apply), title);
    int exit_status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    return exit_status;
}
