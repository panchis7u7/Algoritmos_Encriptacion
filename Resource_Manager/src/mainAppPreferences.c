#include <gtk/gtk.h>

#include "headers/mainApp.h"
#include "headers/mainAppWindow.h"
#include "headers/mainAppPreferences.h"

struct _MainAppPreferences {
  GtkDialog parent;
  GSettings* settings;
  GtkWidget* font;
  GtkWidget* transition;
};

G_DEFINE_TYPE(MainAppPreferences, main_app_preferences, GTK_TYPE_DIALOG)

static void main_app_preferences_inti(MainAppPreferences* preferences){
  gtk_widget_init_template(GTK_WIDGET(preferences));
  preferences->settings = g_settings_new("org.gtk.mainapp");
  g_settings_bind(preferences->settings, "font", preferences->font, "font", G_SETTINGS_BIND_DEFAULT);
  g_settings_bind(preferences->settings, "transition", preferences->transition, "active-id", G_SETTINGS_BIND_DEFAULT);
}

static void main_app_preferences_dispose(GObject* object){
  MainAppPreferences* preferences;
  preferences = MAIN_APP_PREFERENCES(object);
  g_clear_object(&preferences->settings);
  G_OBJECT_CLASS(main_app_preferences_parent_class)->dispose(object);
}

static void main_app_preferences_class_init(MainAppPreferencesClass* class){
  G_OBJECT_CLASS(class)->dispose = main_app_preferences_dispose;
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/org/gtk/mainapp/preferences.ui");
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), MainAppPreferences, font);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), MainAppPreferences, transition);
}

MainAppPreferences* main_app_preferences_new(MainAppWindow* window){
    return g_object_new(MAIN_APP_PREFERENCES_TYPE, "transient-for", window, "use-header-bar", TRUE, NULL);
}