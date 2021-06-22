#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <stdio.h>
#include <string.h>

#include "../headers/mainApp.h"
#include "../headers/mainAppWindow.h"
#include "../headers/fileBrowserView.h"

static void main_app_window_class_init(MainAppWindowClass* class);
static void main_app_window_dispose(GObject* object);
static void main_app_window_init(MainAppWindow* window);
static void command_changed(GtkEntry* entry, MainAppWindow* window);
static void command_submit_pressed(GtkButton *button, MainAppWindow* window);
static GtkTreeModel* create_completion_model(void);

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

struct _MainAppWindow {
    GtkApplicationWindow parent;
    GSettings* settings;
    GtkWidget* gears;
    GtkWidget* commandEntry;
    GtkWidget* btnCommandSubmit;
    GtkWidget* stack;
};

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

G_DEFINE_TYPE (MainAppWindow, main_app_window, GTK_TYPE_APPLICATION_WINDOW)

MainAppWindow* main_app_window_new(MainApp* app){
    return g_object_new(MAIN_APP_WINDOW_TYPE, "application", app, NULL);
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

static void main_app_window_class_init(MainAppWindowClass* class){
  GObjectClass* gobject_class = G_OBJECT_CLASS(class);
  gobject_class->dispose = main_app_window_dispose;

  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/org/gtk/mainapp/ui/window.ui");

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, gears);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, commandEntry);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, btnCommandSubmit);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, stack);

  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (class), command_changed);
  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (class), command_submit_pressed);
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

static void main_app_window_init(MainAppWindow* window){
  GtkBuilder* builder;
  GMenuModel* menu;
  GAction* action;
  GtkEntryCompletion* completion;
  GtkTreeModel *completion_model;
  GtkBox* box;

  gtk_widget_init_template(GTK_WIDGET(window));

  builder = gtk_builder_new_from_resource("/org/gtk/mainapp/ui/gears-menu.ui");
  menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));
  gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(window->gears), menu);

  window->stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack"));

  g_object_unref(builder);

  window->settings = g_settings_new("org.gtk.mainapp");

  //Create completion object.
  completion = gtk_entry_completion_new();

  //Asign the completion to the entry.
  gtk_entry_set_completion(GTK_ENTRY(window->commandEntry), completion);
  g_object_unref(completion);

  //Create a tree model and use it as the completion model.
  completion_model = create_completion_model();
  gtk_entry_completion_set_model(completion, completion_model);
  g_object_unref(completion_model);
  
  /* Use model column 0 as the text column */
  gtk_entry_completion_set_text_column (completion, 0);
  gtk_entry_completion_set_inline_completion (completion, TRUE);
  gtk_entry_completion_set_inline_selection (completion, TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

static void main_app_window_dispose(GObject* object){
  MainAppWindow* window;
  window = MAIN_APP_WINDOW(object);
  g_clear_object(&window->settings);
  G_OBJECT_CLASS(main_app_window_parent_class)->dispose(object);
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

static void command_changed(GtkEntry* entry, MainAppWindow* window){
  
}


///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

static void command_submit_pressed(GtkButton *button, MainAppWindow* window){
  GtkEntryBuffer* buffer = gtk_entry_get_buffer(GTK_ENTRY(window->commandEntry));
  const char* text = gtk_entry_buffer_get_text(buffer);
  char* substring = NULL;

  if((substring = strstr(text, "ls -l")) != NULL){
    g_print("%s", substring);
    char* file = strtok((char*)text, "[");
    g_print("%s", file);
  } else if((substring = strstr(text, "ls")) != NULL){
    g_print("%s", substring);
  } else if ((substring = strstr(text, "cat")) != NULL){
    g_print("%s", substring);
  } else if ((substring = strstr(text, "pwd")) != NULL){
    g_print("%s", substring);
  } else if ((substring = strstr(text, "adduser")) != NULL){
    g_print("%s", substring);
  } else if ((substring = strstr(text, "deluser")) != NULL){
    g_print("%s", substring);
  } else {
    g_print("Not found");
  }

  /*GtkWidget* win;
  win = do_listview_filebrowser(GTK_WIDGET(window));*/
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

static GtkTreeModel* create_completion_model(void){
  const char* commands[] = {
    "ls [dir]",
    "ls -l [dir]",
    "cat [file_path]",
    "pwd",
    "adduser [user_name]",
    "deluser [user_name]",
    NULL
  };

  int i;
  GtkListStore *store;
  GtkTreeIter iter;

  store = gtk_list_store_new (1, G_TYPE_STRING);

  for (i = 0; commands[i]; i++)
    {
      /* Append one word */
      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter, 0, commands[i], -1);
    }

  return GTK_TREE_MODEL (store);
}

///////////////////////////////////////////////////////////////////////////////////////

/* Layout:
 
    +-------------------------------------+
    | +-----------++-------++-----------+ |
    | |  CmdEntry  || Space ||  Submit  | |
    | +-----------++-------++-----------+ |
    +-------------------------------------+
 
  Constraints:
 
    super.start = cmdEntry.start - 8
    cmdEntry.end = space.start
    space.end = Submit.start
    submit.end = super.end - 8

*/
///////////////////////////////////////////////////////////////////////////////////////