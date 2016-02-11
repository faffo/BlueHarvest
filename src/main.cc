#include <gtk/gtk.h>
#include <iostream>
//#include "manipulation.h"

using namespace std;

GtkBuilder *builder;

//Probabilmente da cambiare. Sarebbe lista per dati di una serie, ma lo volevi fare a elem singoli
GSList *list_series;

static int pick;


int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gchar *objects[] = {"combo_genre", "list_series", "w_main", "image1", "entry_completion_name", "entry_completion_genre", NULL};

    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/main.glade", objects, NULL);

    gtk_builder_connect_signals(builder, NULL);
    gtk_main();
    return 0;
}

