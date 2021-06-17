#ifndef _MAIN_APP_WINDOW_H_
#define _MAIN_APP_WINDOW_H_

#include <gtk/gtk.h>
#include "headers/mainApp.h"

#define MAIN_APP_WINDOW_TYPE (main_app_window_get_type ())
G_DECLARE_FINAL_TYPE(MainAppWindow, main_app_window, MAIN, APP_WINDOW, GtkApplicationWindow)

MainAppWindow* main_app_window_new(MainApp* app);
void main_app_window_open(MainAppWindow* window, GFile* file);

#endif //_MAIN_APP_WINDOW_H_