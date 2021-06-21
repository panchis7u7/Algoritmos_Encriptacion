#include <gtk/gtk.h>

#include "../headers/mainApp.h"
#include "../headers/mainAppWindow.h"
#include "../headers/mainAppPreferences.h"

static void main_app_init(MainApp* app);
static void main_app_class_init(MainAppClass* class);
static void main_app_open(GApplication* app, GFile** files, int n_files, const char* hint);
static void preferences_activated(GSimpleAction* action, GVariant* parameter, gpointer app);
static void quit_activated(GSimpleAction* action, GVariant* parameter, gpointer app);
static void main_app_startup(GApplication* app);
static void main_app_activate(GApplication* app);

struct _MainApp {
    GtkApplication parent;
};

G_DEFINE_TYPE(MainApp, main_app, GTK_TYPE_APPLICATION)

MainApp* main_app_new(void) {
    return g_object_new(MAIN_APP_TYPE, "application-id", "org.gtk.mainapp", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}

static void main_app_init(MainApp* app){}

static void main_app_open(GApplication* app, GFile** files, int n_files, const char* hint){
  GList* windows;
  MainAppWindow* window;
  int i;

  windows = gtk_application_get_windows(GTK_APPLICATION(app));
  if(windows)
    window = MAIN_APP_WINDOW(MAIN_APP(app));
  else
    window = main_app_window_new(MAIN_APP(app));
  
  /*for(i = 0; i < n_files; i++)
    main_app_window_open(window, files[i]);*/

   gtk_window_present(GTK_WINDOW(window));
}

static void main_app_class_init(MainAppClass* class){
  G_APPLICATION_CLASS (class)->startup = main_app_startup;
  G_APPLICATION_CLASS (class)->activate = main_app_activate;
  G_APPLICATION_CLASS (class)->open = main_app_open;
}

static void preferences_activated(GSimpleAction* action, GVariant* parameter, gpointer app){
  MainAppPreferences *preferences;
  GtkWindow *window;

  window = gtk_application_get_active_window (GTK_APPLICATION (app));
  preferences = main_app_preferences_new(MAIN_APP_WINDOW(window));
  gtk_window_present (GTK_WINDOW (preferences));
}

static void quit_activated(GSimpleAction* action, GVariant* parameter, gpointer app){
  g_application_quit (G_APPLICATION (app));
}

static GActionEntry app_entries[] = {
  { "preferences", preferences_activated, NULL, NULL, NULL },
  { "quit", quit_activated, NULL, NULL, NULL }
};

static void main_app_startup(GApplication* app){
  const char* quit_accels[2] = {"<Ctrl>Q", NULL};
  G_APPLICATION_CLASS(main_app_parent_class)->startup (app);

  g_action_map_add_action_entries (G_ACTION_MAP (app), app_entries, G_N_ELEMENTS (app_entries), app);
  gtk_application_set_accels_for_action (GTK_APPLICATION (app), "app.quit", quit_accels);
}

static void main_app_activate(GApplication* app){
  MainAppWindow* window;

  window = main_app_window_new(MAIN_APP(app));
  gtk_window_present(GTK_WINDOW(window));
}