#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "../headers/mainApp.h"
#include "../headers/mainAppWindow.h"

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

struct _MainAppWindow {
    GtkApplicationWindow parent;
    GSettings* settings;
    GtkWidget* gears;
    GtkWidget* commandEntry;
    GtkWidget* btnCommandSubmit;
    GtkWidget* stack;
    GtkWidget* flowbox;
    GtkWidget* textView;
};

char* fileImages[] = {
  "/usr/share/icons/Yaru/256x256@2x/status/folder-open.png",
  "/usr/share/icons/Yaru/256x256@2x/mimetypes/text.png"
};

typedef enum {
  SHOW_SIMPLE = 1,
  SHOW_ALL = 2
} lsType;

///////////////////////////////////////////////////////////////////////////////////////

static void main_app_window_class_init(MainAppWindowClass* class);
static void main_app_window_dispose(GObject* object);
static void main_app_window_init(MainAppWindow* window);
static void command_changed(GtkEntry* entry, MainAppWindow* window);
static void command_submit_pressed(GtkButton *button, MainAppWindow* window);
static GtkTreeModel* create_completion_model(void);
void listDirs(MainAppWindow* window, char* directory, lsType type);
void catFile(MainAppWindow* window, char* filePath);

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
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, flowbox);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, textView);

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

  gtk_stack_set_transition_type (GTK_STACK(window->stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(window->flowbox), 6);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(window->flowbox), GTK_SELECTION_SINGLE|GTK_SELECTION_MULTIPLE);

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

  char* directory = (char*)malloc(sizeof(char)*strlen(text));
  strncpy(directory, text, strlen(text)+1);
  char* substring = NULL;

  gtk_flow_box_select_all(GTK_FLOW_BOX(window->flowbox));
  GList* children = gtk_flow_box_get_selected_children(GTK_FLOW_BOX(window->flowbox));
  while(children != NULL){
    gtk_flow_box_remove(GTK_FLOW_BOX(window->flowbox), GTK_WIDGET(children->data));
    children = children->next;
  }

  if((substring = strstr(directory, "ls -la")) != NULL){

    char* rest = (char*)directory;
    char* file = strtok_r((char*)rest, "[", &rest);
    g_print("\nDirectory=> %s\n", rest);
    rest[strlen(rest)-1] = '\0';
    listDirs(window, rest, SHOW_ALL);

  } else if((substring = strstr(directory, "ls")) != NULL){

    char* rest = (char*)directory;
    char* file = strtok_r((char*)rest, "[", &rest);
    g_print("\nDirectory=> %s\n", rest);
    rest[strlen(rest)-1] = '\0';
    listDirs(window, rest, SHOW_SIMPLE);

  } else if ((substring = strstr(text, "cat")) != NULL){
    
    char* rest = (char*)directory;
    char* file = strtok_r((char*)rest, "[", &rest);
    rest[strlen(rest)-1] = '\0';
    g_print("\nDirectory=> %s\n", rest);
    catFile(window, rest);

  } else if ((substring = strstr(text, "pwd")) != NULL){
    g_print("%s", substring);
  } else if ((substring = strstr(text, "adduser")) != NULL){
    g_print("%s", substring);
  } else if ((substring = strstr(text, "deluser")) != NULL){
    g_print("%s", substring);
  } else {
    g_print("Not found");
  }
}

///////////////////////////////////////////////////////////////////////////////////////
//Events.
///////////////////////////////////////////////////////////////////////////////////////

void listDirs(MainAppWindow* window, char* directory, lsType type){
  char buffer[512];
  struct stat buf;
  struct tm creation;
  struct tm modification;
  GtkLabel* label;

  gtk_stack_set_visible_child_name(GTK_STACK(window->stack), "FilesPage");
  DIR* dir;
  struct dirent* ent;
  if ((dir = opendir(directory)) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
      GtkWidget* imagen = gtk_image_new_from_file(fileImages[((ent->d_type == DT_DIR)*0) + ((ent->d_type == DT_REG))]);
      gtk_image_set_pixel_size(GTK_IMAGE(imagen), 64);
      gtk_box_append(GTK_BOX(box), imagen);

      switch(type) {
        case SHOW_SIMPLE:
          gtk_box_append(GTK_BOX(box), gtk_label_new(ent->d_name));
          break;
        case SHOW_ALL:
          if(stat(directory, &buf) >= 0){
            creation = *(gmtime(&buf.st_ctim));
            modification = *(gmtime(&buf.st_mtim));
            snprintf(buffer, 512, "%s\nDueno: %s\nGrupo: %s\nCreacion:\n%d-%d-%d %d:%d:%d\nModificacion:\n%d-%d-%d %d:%d:%d\nAtributos:\n%c%c%c", 
                    ent->d_name, 
                    getpwuid(buf.st_uid)->pw_name, 
                    getgrgid(buf.st_gid)->gr_name,
                    creation.tm_mday, creation.tm_mon, creation.tm_year + 1900, creation.tm_hour, creation.tm_min, creation.tm_sec,
                    modification.tm_mday, modification.tm_mon, modification.tm_year + 1900, modification.tm_hour, modification.tm_min, modification.tm_sec,
                    ((buf.st_mode & R_OK) ? 'r' : '-'),
                    ((buf.st_mode & W_OK) ? 'w' : '-'),
                    ((buf.st_mode & X_OK) ? 'x' : '-'));

            label = GTK_LABEL(gtk_label_new(buffer));
            gtk_label_set_justify(label, GTK_JUSTIFY_CENTER);
            gtk_box_append(GTK_BOX(box), GTK_WIDGET(label));
          } else {
            label = GTK_LABEL(gtk_label_new(ent->d_name));
            gtk_label_set_justify(label, GTK_JUSTIFY_CENTER);
            gtk_box_append(GTK_BOX(box), GTK_WIDGET(label));
          }
          break;
      };
      gtk_flow_box_insert(GTK_FLOW_BOX(window->flowbox), box, -1);
    }
    closedir (dir);
  } else {
    gtk_stack_set_visible_child_name(GTK_STACK(window->stack), "ErrorPage");
  }
}

///////////////////////////////////////////////////////////////////////////////////////

void catFile(MainAppWindow* window, char* filePath){
  FILE* fp;
  struct stat st;
  int len;
  char c;
  char* fileBuffer;
  gtk_stack_set_visible_child_name(GTK_STACK(window->stack), "CatPage");
  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(window->textView));
  if((fp = fopen(filePath, "r")) != NULL){
    stat(filePath, &st);
    len = st.st_size;
    g_print("%d", len);
    fileBuffer = (char*)malloc(sizeof(char)*len);
    size_t newLen = fread(fileBuffer, sizeof(char), len, fp);
    g_print("%s", fileBuffer);
    gtk_text_buffer_set_text(buffer, fileBuffer, len);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(window->textView), buffer);
    free(fileBuffer);
  } else {
    gtk_stack_set_visible_child_name(GTK_STACK(window->stack), "ErrorPage");
  }
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

static GtkTreeModel* create_completion_model(void){
  const char* commands[] = {
    "ls [dir]",
    "ls -la [dir]",
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