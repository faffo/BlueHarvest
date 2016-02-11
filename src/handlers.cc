//
// Created by faffo on 03/02/16.
//

#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "handlers.h"
#include "gtkfun.h"
#include "loadsave.h"
//#include "manipulation.h"
//#include "struct.h"
//#include <iostream>

using namespace std;

extern GtkBuilder *builder;

//extern series *s;

GSList *l_series = NULL;


extern "C" void handler_save_file(GtkButton *gtkButton, gpointer p_fc_widget) {

    GtkTreeModel *treeModel = GTK_TREE_MODEL(gtk_builder_get_object(builder, "list_series"));

    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(p_fc_widget);
    const char *fname = gtk_file_chooser_get_filename(fileChooser);

    cout << fname << endl;
    GSList** plist = &l_series;
    gtk_tree_model_foreach(treeModel, read_list, NULL /*(gpointer) plist*/);

    series *s = (series *) l_series->data;
    cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " <<
    s->year << " " << s->genre << " " << s->status << " " << s->watched << endl;
/*
    series *next = (series *) l_series->next;
    GSList *tmp = l_series;
    series *t = (series *) tmp->data;

    while (tmp) {
        cout << t->name << " " << t->last_episode << " " << t->n_episodes << " " << t->n_seasons << " " <<
        t->year << " " << t->genre << " " << t->status << " " << t->watched << endl;
        tmp = g_slist_next(tmp);
    }
*/
    g_slist_foreach(l_series, (GFunc) print, NULL);

    cout << "list for each print PRIMA di andare in save" << endl;
    g_slist_foreach(l_series, (GFunc)print, NULL);
    cout << "******************" << endl;
    save(l_series, fname);

}


extern "C" void handler_sort(GtkMenuItem *menuItem, gpointer tree) {
    GtkTreeModel *treeModel = GTK_TREE_MODEL(tree);
    if(l_series!=NULL){
        g_slist_foreach(l_series, (GFunc)g_free, NULL);
        g_slist_free(l_series);
        l_series = NULL;
    }
    gtk_tree_model_foreach(treeModel, read_list, NULL);
    gtk_list_store_clear(GTK_LIST_STORE(tree));
    const gchar *label = gtk_menu_item_get_label(menuItem);
    int selection = sort_menu_conv(label);
    l_series = sort(l_series, selection);


    g_slist_foreach(l_series, (GFunc) refresh_treeview, (gpointer) GTK_LIST_STORE(tree));
}

void destroy_widget(GtkWidget *widget){
    gtk_widget_destroy(widget);
}

extern "C" void handler_show_search(GtkMenuItem *menuItem, gpointer p_header){
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_name")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry")), "");
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_genre")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry")), "");

    GtkWidget *header_search = GTK_WIDGET(p_header);
    gtk_widget_show_all(header_search);
}

extern "C" void handler_search(GtkButton *button, gpointer plabel){
    gchar *objects[] = {"w_found", "list_found", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/main.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);

    GSList *l_found = NULL;
    if(l_series!=NULL){
        g_slist_foreach(l_series, (GFunc)g_free, NULL);
        g_slist_free(l_series);
        l_series = NULL;
    }
    const char *label = gtk_label_get_text(GTK_LABEL(plabel));
    int selection = search_conv(label);
    const char* text = get_entry(search_conv(label));
    GtkTreeModel *treeModel = GTK_TREE_MODEL(gtk_builder_get_object(builder, "list_series"));

    gtk_tree_model_foreach(treeModel, read_list, NULL);
    search(l_series, l_found, selection, text);
    GtkListStore *listfound = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_found"));
    g_slist_foreach(l_found, (GFunc) refresh_treeview, (gpointer) listfound);
}

extern "C" void handler_delete_widget(GtkButton *gtkButton, gpointer widget) {
    destroy_widget(GTK_WIDGET(widget));
}

extern "C" void handler_delete_dialog(GtkWidget *gtkDialog, gpointer widget) {
}

extern "C" void handler_add_series(GtkButton *gtkButton, gpointer add_series) {

    GtkGrid *data_grid = (GtkGrid *) add_series;
    //name
    GtkEntry *entry_series_name = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 0);
    const gchararray series_name = (gchararray const) gtk_entry_get_text(entry_series_name);
    //last ep
    GtkEntry *entry_series_last_ep = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 1);
    const gchararray series_last_ep = (gchararray const) gtk_entry_get_text(entry_series_last_ep);
    //numero ep
    GtkEntry *entry_series_num_ep = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 2);
    const gchar *series_num_ep_string = gtk_entry_get_text(entry_series_num_ep);
    const gint series_num_ep = atoi(series_num_ep_string);
    //numero seas
    GtkEntry *entry_series_num_seas = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 3);
    const gchar *series_num_seas_string = gtk_entry_get_text(entry_series_num_seas);
    const gint series_num_seas = atoi(series_num_seas_string);
    //anno
    GtkEntry *entry_series_year = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 4);
    const gchar *series_year_string = gtk_entry_get_text(entry_series_year);
    const gint series_year = atoi(series_year_string);
    //genere
    GtkComboBoxText *combo_genre = (GtkComboBoxText *) gtk_grid_get_child_at(data_grid, 1, 5);
    const gchararray genre_string = (gchararray const) gtk_combo_box_text_get_active_text(combo_genre);

    //sottogriglia dei toggle
    GtkGrid *toggle_grid = (GtkGrid *) gtk_grid_get_child_at(data_grid, 1, 6);

    char *status_string;
    char *watched_string;

    //finita o in corso
    GtkToggleButton *toggle_ended = (GtkToggleButton *) gtk_grid_get_child_at(toggle_grid, 0, 0);
    gboolean status = gtk_toggle_button_get_active(toggle_ended);
    if (status) status_string = "Yes";
    else status_string = "No";
    //vista o no
    GtkToggleButton *toggle_watched = (GtkToggleButton *) gtk_grid_get_child_at(toggle_grid, 1, 0);
    gboolean watched = gtk_toggle_button_get_active(toggle_watched);
    if (watched) watched_string = "Yes";
    else watched_string = "No";

    /*
    cout << series_name << endl;
    cout << series_last_ep << endl;
    cout << series_num_ep << endl;
    cout << series_num_seas << endl;
    cout << series_year << endl;
    cout << genre_string << endl;
    cout << status_string << endl;
    cout << watched_string << endl;
    cout << "**********" << endl;
*/

    series *s = data_to_struct(series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string,
                       status_string, watched_string);
    //insert_series(l_series,series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string, status_string, watched_string);
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    refresh_treeview(s, (gpointer) listStore);

    //refresh_treeview(series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string, status_string, watched_string);


}

extern "C" void handler_save(GtkButton *gtkButton, gpointer save_dialog_pointer) {
    GtkFileChooserDialog *save_chooser = (GtkFileChooserDialog *) save_dialog_pointer;

}

extern "C" void handler_open_file(GtkButton *gtkButton, gpointer p_fc_widget) {

    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    gtk_list_store_clear(listStore);

    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(p_fc_widget);
    const char *fname = gtk_file_chooser_get_filename(fileChooser);


    load(l_series, fname);
    g_slist_foreach(l_series, (GFunc) refresh_treeview, (gpointer) listStore);
}

/**Handlers per la generazione delle finestre*/
extern "C" void handler_add(GtkMenuItem *menuItem, gpointer dialog_add) {
    //gtk_widget_show_all(GTK_WIDGET(dialog_add));
    gchar *objects[] = {"w_add_series", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/main.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
    //gtk_builder_add_from_file(dialog, "../src/resources/glade/dialog.glade", NULL);

}

extern "C" void handler_dialog_open(GtkMenuItem *menuItem, gpointer file_choser_open) {
    gchar *objects[] = {"w_open", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/main.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}

extern "C" void handler_dialog_save_as(GtkMenuItem *menuItem, gpointer file_chooser_save) {
    gchar *objects[] = {"w_save", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/main.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}

extern "C" void handler_dialog_delete_series(GtkMenuItem *menuItem, gpointer dialog_delete_series) {
    gtk_widget_show_all(GTK_WIDGET(dialog_delete_series));
}