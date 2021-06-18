#ifndef _MAIN_APP_PREFERENCES_H_
#define _MAIN_APP_PREFERENCES_H_

#include <gtk/gtk.h>
#include "../headers/mainAppWindow.h"

#define MAIN_APP_PREFERENCES_TYPE (main_app_preferences_get_type ())
G_DECLARE_FINAL_TYPE(MainAppPreferences, main_app_preferences, MAIN, APP_PREFERENCES, GtkDialog)

MainAppPreferences* main_app_preferences_new(MainAppWindow* window);

#endif //_MAIN_APP_PREFERENCES_H_