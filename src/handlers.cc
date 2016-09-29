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

using namespace std;

extern GtkBuilder *builder;

//extern series *s;

GSList *l_series = NULL;
GSList *l_found = NULL;

void destroy_widget(GtkWidget *widget){
    gtk_widget_destroy(widget);
}

extern "C" void handler_delete_widget(GtkButton *gtkButton, gpointer widget) {
    destroy_widget(GTK_WIDGET(widget));
}

extern "C" void handler_save_file(GtkButton *gtkButton, gpointer p_fc_widget) {

    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(p_fc_widget);
    const char *fname = gtk_file_chooser_get_filename(fileChooser);

    save(l_series, fname);

}

extern "C" void handler_sort(GtkMenuItem *menuItem, gpointer tree) {
    GtkTreeModel *treeModel = GTK_TREE_MODEL(tree);
    /*
    if(l_series!=NULL){
        g_slist_foreach(l_series, (GFunc)g_free, NULL);
        g_slist_free(l_series);
        l_series = NULL;
    }
    gtk_tree_model_foreach(treeModel, read_list, (gpointer) &l_series);
    */
    gtk_list_store_clear(GTK_LIST_STORE(tree));
    const gchar *label = gtk_menu_item_get_label(menuItem);
    int selection = sort_menu_conv(label);
    l_series = sort(l_series, selection);


    g_slist_foreach(l_series, (GFunc) refresh_treeview, (gpointer) GTK_LIST_STORE(tree));
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
    //gchar *objects[] = {"w_found", "list_found", NULL};
    //gtk_builder_add_objects_from_file(builder, "../src/resources/glade/dialog.glade", objects, NULL);
    //gtk_builder_connect_signals(builder, NULL);

    if(l_found!=NULL){
        g_slist_foreach(l_found, (GFunc)g_free, (gpointer) NULL);
        g_slist_free(l_found);
        l_found = NULL;
    }

    gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(builder, "w_found")));

    const char *label = gtk_label_get_text(GTK_LABEL(plabel));
    int selection = search_conv(label);
    const char* text = get_entry(search_conv(label));
    GtkTreeModel *treeModel = GTK_TREE_MODEL(gtk_builder_get_object(builder, "list_series"));

    //gtk_tree_model_foreach(treeModel, read_list, (gpointer) &l_found);
    //l_found = search(l_found, selection, text);
    l_found = g_slist_copy(l_series);
    search(l_found, selection, text);
    GtkListStore *listfound = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_found"));
    gtk_list_store_clear(listfound);
    g_slist_foreach(l_found, (GFunc) refresh_treeview, (gpointer) listfound);

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_name")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry")), "");
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_genre")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry")), "");
}

extern "C" void handler_search_name(GtkButton *button, gpointer pname){
    gchar *objects[] = {"w_found", "list_found", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/dialog.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);

    g_slist_foreach(l_found, (GFunc)g_free, NULL);
    g_slist_free(l_found);
    l_found = NULL;

    const gchar* s_name = gtk_entry_get_text(GTK_ENTRY(pname));
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_found"));

    g_slist_foreach(l_series, (GFunc) sname, (gpointer) s_name);

    //g_slist_foreach(l_found, (GFunc) print_name_test, NULL);

    g_slist_foreach(l_found, (GFunc) refresh_treeview, (gpointer) listStore);

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_name")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry")), "");
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_genre")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry")), "");
}

extern "C" void handler_search_genre(GtkButton *button, gpointer pgenre){
    gchar *objects[] = {"w_found", "list_found", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/dialog.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);

    g_slist_foreach(l_found, (GFunc)g_free, NULL);
    g_slist_free(l_found);
    l_found = NULL;

    const gchar* s_genre = gtk_entry_get_text(GTK_ENTRY(pgenre));
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_found"));
    cout << s_genre << endl;

    g_slist_foreach(l_series, (GFunc) sgenre, (gpointer) s_genre);

    g_slist_foreach(l_found, (GFunc) refresh_treeview, (gpointer) listStore);

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_name")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry")), "");
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_genre")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry")), "");
}

extern "C" void handler_delete(GtkButton *button, gpointer ptw_found) {
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptw_found));
    //tw_edit(sel, DEL_MODE);
    GtkTreeModel *treeModel = GTK_TREE_MODEL(gtk_builder_get_object(builder, "list_series"));
    series *s = get_sel(sel);

    n_search_gtk *search_data = new n_search_gtk;
    char *name = s->name;
    search_data->name = name;
    search_data->mode = DEL_MODE;
    cout << "test search_data-> mode in handler_delete: " << search_data->mode << endl;

    gtk_tree_model_foreach(treeModel, find_tw, (gpointer) search_data);

    gtk_tree_model_foreach(treeModel, del, (gpointer) search_data);
    l_series = g_slist_remove(l_series, s);
    destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_found")));
}

extern "C" void handler_edit(GtkButton *button, gpointer ptw_found){
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptw_found));
    GtkTreeModel *treeModel = GTK_TREE_MODEL(gtk_builder_get_object(builder, "list_series"));
    //tw_edit(sel, EDIT_MODE);
    series *s = get_sel(sel);
    dialog_edit(s, treeModel);
}

extern "C" void handler_edit_confirm(GtkButton *button, gpointer edit_series){
    cout << "DENTRO HANDLER_EDIT_CONFIRM" << endl;
    //Inizializzo treeview found e series oltre che la selezione
    GtkTreeView *treeViewfound = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tw_found"));
    GtkTreeView *treeViewSeries = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tw_series"));
    GtkTreeModel *treeModel = GTK_TREE_MODEL((gtk_builder_get_object(builder, "list_series")));
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    GtkTreeSelection *sel = gtk_tree_view_get_selection(treeViewfound);
    //Estrappolo la serie dalla selezione
    series *s = get_sel(sel);
    char *name = s->name;
    //creo iter
    GtkTreeIter *iter = NULL;
    //prendo dati dalla grid
    GtkGrid *data_grid = (GtkGrid *) edit_series;

    series *edited = grid_to_struct(data_grid);

    cout << "test contenuto nome edited->name: " << edited->name << endl;

    //creo struct per passaggio info a funzione find
    n_search_gtk *search_data = new n_search_gtk;
    search_data->name = name;
    search_data->mode = EDIT_MODE;
    search_data->edited = edited;

    //eseguo treeview foreach
    //gtk_tree_model_foreach(treeModel, find_tw, (gpointer) search_data);

    gtk_list_store_clear(listStore);

    int index = g_slist_index(l_series, s);
    l_series = g_slist_remove_all(l_series, s);
    l_series = g_slist_insert(l_series, edited, index);
    g_slist_foreach(l_series, (GFunc) refresh_treeview, (gpointer) listStore);

    //gtk_tree_selection_get_selected(sel, (GtkTreeModel **) treeViewfound, iter);
    //gtk_list_store_set(GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series")), iter);

    gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "w_edit")));
}

extern "C" void handler_add_series(GtkButton *gtkButton, gpointer add_series) {

    GtkGrid *data_grid = (GtkGrid *) add_series;
    /*
    //name
    GtkEntry *entry_series_name = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 0);
    const gchararray series_name = (gchararray const) gtk_entry_get_text(entry_series_name);
    //last ep
    GtkEntry *entry_series_last_ep = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 1);
    const gchararray series_last_ep = (gchararray const) gtk_entry_get_text(entry_series_last_ep);
    //numero ep
    GtkEntry *entry_series_num_ep = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 2);
    const gchar *series_num_ep_string = gtk_entry_get_text(entry_series_num_ep);
    const gint series_num_ep = stoi(series_num_ep_string);
    //numero seas
    GtkEntry *entry_series_num_seas = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 3);
    const gchar *series_num_seas_string = gtk_entry_get_text(entry_series_num_seas);
    const gint series_num_seas = stoi(series_num_seas_string);
    //anno
    GtkEntry *entry_series_year = (GtkEntry *) gtk_grid_get_child_at(data_grid, 1, 4);
    const gchar *series_year_string = gtk_entry_get_text(entry_series_year);
    const gint series_year = stoi(series_year_string);
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
    /*
    series *s = data_to_struct(series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string,
                       status_string, watched_string);
    */

    series *s = grid_to_struct(data_grid);
    //insert_series(l_series,series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string, status_string, watched_string);
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    refresh_treeview(s, (gpointer) listStore);

    //refresh_treeview(series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string, status_string, watched_string);

    l_series = g_slist_append(l_series, s);

    destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_add_series")));
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
    destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_open")));
}

/**Handlers per la generazione delle finestre*/
extern "C" void handler_add(GtkMenuItem *menuItem, gpointer dialog_add) {
    //gtk_widget_show_all(GTK_WIDGET(dialog_add));
    gchar *objects[] = {"w_add_series", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/dialog.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
    //gtk_builder_add_from_file(dialog, "../src/resources/glade/dialog.glade", NULL);

}

extern "C" void handler_dialog_open(GtkMenuItem *menuItem, gpointer file_choser_open) {
    gchar *objects[] = {"w_open", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/dialog.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}

extern "C" void handler_dialog_save_as(GtkMenuItem *menuItem, gpointer file_chooser_save) {
    gchar *objects[] = {"w_save", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/dialog.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}

extern "C" void handler_dialog_delete_series(GtkMenuItem *menuItem, gpointer dialog_delete_series) {
    gtk_widget_show_all(GTK_WIDGET(dialog_delete_series));
}