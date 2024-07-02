#include <stdio.h>
#include <gtk/gtk.h>

static void apply(GtkApplication *app, gpointer data) {
    char *title = (char*) data;
    printf("Starting application\n");
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
        fprintf(stderr, "Window could not be created\n");
        return;
    }
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
    g_signal_connect(app, "activate", G_CALLBACK(apply), argc > 1 ? argv[1] : "Default title");
    int exit_status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    if (exit_status == 0) {
        printf("App closed successfully!\n");
    } else {
        fprintf(stderr, "App exited with code %d\n", exit_status);
    }

    return exit_status;
}
