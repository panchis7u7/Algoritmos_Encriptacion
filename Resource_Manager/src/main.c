#include <gtk/gtk.h>
#include "headers/mainapp.h"

int main(int argc, char* argv[]){
    
    g_setenv("GSETTINGS_SCHEMA_DIR", ".", FALSE);

    return g_application_run(G_APPLICATION(main_app_new()), argc, argv);
}