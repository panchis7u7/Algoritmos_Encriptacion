#ifndef _FILE_BROWSER_VIEW_H_
#define _FILE_BROWSER_VIEW_H_

#include <gtk/gtk.h>
#include "../headers/mainAppWindow.h"

#define FILE_BROWSER_TYPE (file_browser_view_get_type ())
G_DECLARE_FINAL_TYPE(FileBrowserView, file_browser_view, FILE_BROWSER, VIEW, GObject)

FileBrowserView* file_browser_view_new(GtkWindow* window);
GtkWidget* do_listview_filebrowser (GtkWidget *do_widget);

#endif //_FILE_BROWSER_VIEW_H_