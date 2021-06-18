#include <gtk/gtk.h>

#include "../headers/mainApp.h"
#include "../headers/mainAppWindow.h"

static void main_app_window_class_init(MainAppWindowClass* class);
static void main_app_window_dispose(GObject* object);
static void main_app_window_init(MainAppWindow* window);
static void find_word(GtkButton* button, MainAppWindow* window);
static void update_words(MainAppWindow* window);
static void update_lines(MainAppWindow* window);
static void words_changed(GObject* sideBar, GParamSpec* pspec, MainAppWindow* window);
static void visible_child_changed(GObject* stack, GParamSpec* pspec, MainAppWindow* window);
static void search_text_changed(GtkEntry* entry, MainAppWindow* window);

struct _MainAppWindow {
    GtkApplicationWindow parent;
    GSettings* settings;
    GtkWidget* stack;
    GtkWidget* gears;
    GtkWidget* search;
    GtkWidget* searchBar;
    GtkWidget* searchEntry;
    GtkWidget* sideBar;
    GtkWidget* words;
    GtkWidget* lines;
    GtkWidget* lines_label;
};

G_DEFINE_TYPE (MainAppWindow, main_app_window, GTK_TYPE_APPLICATION_WINDOW)

MainAppWindow* main_app_window_new(MainApp* app){
    return g_object_new(MAIN_APP_WINDOW_TYPE, "application", app, NULL);
}

static void main_app_window_class_init(MainAppWindowClass* class){
  G_OBJECT_CLASS(class)->dispose = main_app_window_dispose;
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/org/gtk/exampleapp/window.ui");

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, stack);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, gears);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, search);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, searchBar);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, searchEntry);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, words);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, sideBar);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, lines);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), MainAppWindow, lines_label);

  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (class), search_text_changed);
  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (class), visible_child_changed);
}

static void main_app_window_init(MainAppWindow* window){
  GtkBuilder* builder;
  GMenuModel* menu;
  GAction* action;

  gtk_widget_init_template(GTK_WIDGET(window));

  builder = gtk_builder_new_from_resource("/org/gtk/exampleapp/gears-menu.ui");
  menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));
  gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(window->gears), menu);
  g_object_unref(builder);

  window->settings = g_settings_new("org.gtk.exampleapp");

  g_settings_bind(window->settings, "transition", window->stack, "transition-type", G_SETTINGS_BIND_DEFAULT);
  g_settings_bind(window->settings, "show-words", window->sideBar, "reveal-child", G_SETTINGS_BIND_DEFAULT);
  g_object_bind_property(window->search, "active", window->searchBar, "search-mode-enabled", G_BINDING_BIDIRECTIONAL);
  g_signal_connect(window->sideBar, "notify::reveal-child", G_CALLBACK(words_changed), window);

  action = g_settings_create_action(window->settings, "show-words");
  g_action_map_add_action(G_ACTION_MAP(window), action);
  g_object_unref(action);

  action = (GAction*) g_property_action_new("show-lines", window->lines, "visible");
  g_action_map_add_action(G_ACTION_MAP(window), action);
  g_object_unref(action);

  g_object_bind_property(window->lines, "visible", window->lines_label, "visible", G_BINDING_DEFAULT);
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
  gtk_stack_add_titled (GTK_STACK(window->stack), scrolled, basename, basename);

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

  gtk_widget_set_sensitive (window->search, TRUE);

  update_words (window);
  update_lines (window);
}

static void main_app_window_dispose(GObject* object){
  MainAppWindow* window;
  window = MAIN_APP_WINDOW(object);
  g_clear_object(&window->settings);
  G_OBJECT_CLASS(main_app_window_parent_class)->dispose(object);
}

static void find_word(GtkButton* button, MainAppWindow* window){
  const char* word;
  word = gtk_button_get_label(button);
  gtk_editable_set_text(GTK_EDITABLE(window->searchEntry), word);
}

static void update_words(MainAppWindow* window){
  GHashTable *strings;
  GHashTableIter iter;
  GtkWidget *tab, *view, *row;
  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  char *word, *key;
  GtkWidget *child;

  tab = gtk_stack_get_visible_child (GTK_STACK(window->stack));

  if (tab == NULL)
    return;

  view = gtk_scrolled_window_get_child (GTK_SCROLLED_WINDOW (tab));
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

  strings = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);

  gtk_text_buffer_get_start_iter (buffer, &start);
  while (!gtk_text_iter_is_end (&start))
    {
      while (!gtk_text_iter_starts_word (&start))
        {
          if (!gtk_text_iter_forward_char (&start))
            goto done;
        }
      end = start;
      if (!gtk_text_iter_forward_word_end (&end))
        goto done;
      word = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
      g_hash_table_add (strings, g_utf8_strdown (word, -1));
      g_free (word);
      start = end;
    }

done:
  while ((child = gtk_widget_get_first_child (window->words)))
    gtk_list_box_remove (GTK_LIST_BOX(window->words), child);

  g_hash_table_iter_init (&iter, strings);
  while (g_hash_table_iter_next (&iter, (gpointer *)&key, NULL))
    {
      row = gtk_button_new_with_label (key);
      g_signal_connect(row, "clicked", G_CALLBACK (find_word), window);
      gtk_list_box_insert (GTK_LIST_BOX(window->words), row, -1);
    }

  g_hash_table_unref (strings);
}

static void update_lines(MainAppWindow* window){
  GtkWidget *tab, *view;
  GtkTextBuffer *buffer;
  int count;
  char *lines;

  tab = gtk_stack_get_visible_child (GTK_STACK (window->stack));

  if (tab == NULL)
    return;

  view = gtk_scrolled_window_get_child (GTK_SCROLLED_WINDOW (tab));
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

  count = gtk_text_buffer_get_line_count (buffer);
  lines = g_strdup_printf ("%d", count);
  gtk_label_set_text (GTK_LABEL(window->lines), lines);
  g_free (lines);
}

static void words_changed(GObject* sideBar, GParamSpec* pspec, MainAppWindow* window){
  update_words(window);
}

static void visible_child_changed(GObject* stack, GParamSpec* pspec, MainAppWindow* window){
  if (gtk_widget_in_destruction (GTK_WIDGET (stack)))
    return;

  gtk_search_bar_set_search_mode (GTK_SEARCH_BAR(window->searchBar), FALSE);
  update_words (window);
  update_lines (window);
}

static void search_text_changed(GtkEntry* entry, MainAppWindow* window){
const char *text;
  GtkWidget *tab;
  GtkWidget *view;
  GtkTextBuffer *buffer;
  GtkTextIter start, match_start, match_end;

  text = gtk_editable_get_text (GTK_EDITABLE (entry));

  if (text[0] == '\0')
    return;

  tab = gtk_stack_get_visible_child (GTK_STACK(window->stack));
  view = gtk_scrolled_window_get_child (GTK_SCROLLED_WINDOW (tab));
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

  /* Very simple-minded search implementation */
  gtk_text_buffer_get_start_iter (buffer, &start);
  if (gtk_text_iter_forward_search (&start, text, GTK_TEXT_SEARCH_CASE_INSENSITIVE,
                                    &match_start, &match_end, NULL))
    {
      gtk_text_buffer_select_range (buffer, &match_start, &match_end);
      gtk_text_view_scroll_to_iter (GTK_TEXT_VIEW (view), &match_start,
                                    0.0, FALSE, 0.0, 0.0);
    }
}