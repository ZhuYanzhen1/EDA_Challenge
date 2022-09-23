/**************************************************************************//**
  \file     gui.cc
  \brief    This document describes the interface of the main form.
  \author   Lao·Zhu
  \version  V1.0.1
  \date     23. September 2022
 ******************************************************************************/

#include "gui.h"
#include <gtk/gtk.h>

static void gui_button_callback(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

static void gui_app_active(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Window");
    gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);

    button = gtk_button_new_with_label("Hello World");
    g_signal_connect (button, "clicked", G_CALLBACK(gui_button_callback), NULL);
    gtk_window_set_child(GTK_WINDOW (window), button);

    gtk_window_present(GTK_WINDOW (window));
}

int gui_display_main_window(int argc, char **argv) {
    GtkApplication *app;
    app = gtk_application_new("eda.statistic.tools", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK(gui_app_active), NULL);
    int status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);
    return status;
}