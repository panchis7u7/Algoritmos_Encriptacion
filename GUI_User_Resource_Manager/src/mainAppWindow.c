#include <gtk/gtk.h>

#include "../headers/mainApp.h"
#include "../headers/mainAppWindow.h"

static void main_app_window_class_init(MainAppWindowClass* class);
static void main_app_window_dispose(GObject* object);
static void main_app_window_init(MainAppWindow* window);

struct _MainAppWindow {
    GtkApplicationWindow parent;
    GSettings* settings;
    GtkWidget* title;
    GtkWidget* gears;
};

G_DEFINE_TYPE (MainAppWindow, main_app_window, GTK_TYPE_APPLICATION_WINDOW)

MainAppWindow* main_app_window_new(MainApp* app){
    return g_object_new(MAIN_APP_WINDOW_TYPE, "application", app, NULL);
}

static void main_app_window_class_init(MainAppWindowClass* class){
  G_OBJECT_CLASS(class)->dispose = main_app_window_dispose;
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/org/gtk/mainapp/ui/window.ui");

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, title);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, gears);
}

static void main_app_window_init(MainAppWindow* window){
  GtkBuilder* builder;
  GMenuModel* menu;
  GAction* action;

  gtk_widget_init_template(GTK_WIDGET(window));

  builder = gtk_builder_new_from_resource("/org/gtk/mainapp/ui/gears-menu.ui");
  menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));
  gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(window->gears), menu);
  g_object_unref(builder);

  window->settings = g_settings_new("org.gtk.mainapp");
}

void main_app_window_open(MainAppWindow* window, GFile* file){
  char *basename;
  GtkWidget *scrolled, *view;
  char *contents;
  gsize length;
  GtkTextBuffer *buffer;
  GtkTextTag *tag;
  GtkTextIter start_iter, end_iter;

  basename = g_file_get_basename (file);

  scrolled = gtk_scrolled_window_new ();
  gtk_widget_set_hexpand (scrolled, TRUE);
  gtk_widget_set_vexpand (scrolled, TRUE);
  view = gtk_text_view_new ();
  gtk_text_view_set_editable (GTK_TEXT_VIEW (view), FALSE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (view), FALSE);
  gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scrolled), view);

  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

  if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL))
    {
      gtk_text_buffer_set_text (buffer, contents, length);
      g_free (contents);
    }

  tag = gtk_text_buffer_create_tag (buffer, NULL, NULL);
  g_settings_bind (window->settings, "font",
                   tag, "font",
                   G_SETTINGS_BIND_DEFAULT);

  gtk_text_buffer_get_start_iter (buffer, &start_iter);
  gtk_text_buffer_get_end_iter (buffer, &end_iter);
  gtk_text_buffer_apply_tag (buffer, tag, &start_iter, &end_iter);

  g_free (basename);
}

static void main_app_window_dispose(GObject* object){
  MainAppWindow* window;
  window = MAIN_APP_WINDOW(object);
  g_clear_object(&window->settings);
  G_OBJECT_CLASS(main_app_window_parent_class)->dispose(object);
}