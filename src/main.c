#include <stdio.h>
#include <gtk/gtk.h>

static void apply(GtkApplication *app, gpointer data) {
    (void) data;
    printf("Starting application\n");
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Bezier");
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
    g_signal_connect(app, "activate", G_CALLBACK(apply), NULL);
    int exit_status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return exit_status;
}
