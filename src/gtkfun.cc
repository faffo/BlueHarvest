//
// Created by faffo on 03/02/16.
//

#include <gtk/gtk.h>
#include <stdlib.h>
#include "gtkfun.h"

using namespace std;

extern GtkBuilder *builder;
extern GSList *l_series;
GSList *l_sel = NULL;


void refresh_treeview(series *s, gpointer plist) {
    cout << "dentro refresh_treeview" << endl;
    cout << s->name << endl;
    GtkListStore *listStore = GTK_LIST_STORE(plist);

    GtkTreeIter iter;
    //GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));

    gtk_list_store_append(listStore, &iter);
    gtk_list_store_set(listStore, &iter, 0, s->name,
                       1, s->last_episode,
                       2, s->n_episodes,
                       3, s->n_seasons,
                       4, s->year,
                       5, s->genre,
                       6, status_int2text(s->status),
                       7, watched_int2text(s->watched),
                       -1);
}
/*
void tw_edit(GtkTreeSelection *sel, int mode) {
    GtkTreeModel *treeModel = GTK_TREE_MODEL(gtk_builder_get_object(builder, "list_series"));
    GtkTreeModel *model_edit;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(sel, &model_edit, &iter)) {
        series *s = tw_to_struct(model_edit, &iter);
        switch (mode) {
            case EDIT_MODE:
                dialog_edit(s, treeModel);
                break;
            case DEL_MODE:
                gtk_tree_model_foreach(treeModel, del, (gpointer) s);
                break;
        }

    }
    //gtk_tree_model_foreach(model_edit, read_list, (gpointer) &l_sel);
    //gtk_tree_model_foreach(treeModel, del, (gpointer) &l_sel);
}
*/
series * get_sel(GtkTreeSelection *sel){
    GtkTreeModel *model_sel;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(sel, &model_sel, &iter))
        return tw_to_struct(model_sel, &iter);
}

void dialog_edit(series *s, GtkTreeModel *treemodel){
    gchar* objects[] = {"w_edit", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/dialog.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);

    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_name"))), s->name);
    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_last_ep"))), s->last_episode);
    //convert int to text n_ep
    char n_ep_text[] = "";
    cout << "contenuto n_ep_text 1:" << n_ep_text << endl;
    strcpy(n_ep_text, itochar(s->n_episodes));
    cout << "contenuto n_ep_text 2:" << n_ep_text << endl;
    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_n_ep"))), n_ep_text);
    //convert int to text n_seasons
    char n_seas_text[] = "";
    cout << "contenuto n_seas_text 1:" << n_seas_text << endl;
    strcpy(n_seas_text, itochar(s->n_seasons));
    cout << "contenuto n_seas_text 2:" << n_seas_text << endl;
    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_n_seas"))), n_seas_text);
    //convert int to text year
    char year_text[] = "";
    cout << "contenuto year_text 1:" << year_text << endl;
    strcpy(year_text, itochar(s->year));
    cout << "contenuto year_text 2:" << year_text << endl;
    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_year"))), year_text);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX((gtk_builder_get_object(builder, "edit_combo_genre"))), s->genre);

    if(s->status){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "edit_checkb_status")), true);
    }
    if(s->watched){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "edit_checkb_watched")), true);
    }

}

void back_to_main(){
    gtk_list_store_clear(GTK_LIST_STORE(gtk_builder_get_object(builder, "list_found")));
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "foundbox")));
    gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(builder, "tw_series")));
}

gboolean read_list(GtkTreeModel *model,
                   GtkTreePath *path,
                   GtkTreeIter *iter,
                   gpointer data) {

    //gchararray status, watched, name, last_episode, genre;
    //gint n_episodes, n_seasons, year;
    GSList **pList = (GSList **) data;
    GSList *&list = *pList;

    gchar *tree_path_string;
    tree_path_string = gtk_tree_path_to_string(path);
    cout << tree_path_string << endl;
    series *s = tw_to_struct(model, iter);
    //cout << "dopo data_to_struct dentro read_list"<< endl;
    //cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year << " " << s->genre << " " << s->status << " " << s->watched << endl;
    insert_series(list, s);
    series *prova = (series *) list->data;
    cout << "valore tornato a read_list " << prova->name << endl;


    return FALSE;
}

const char *get_entry(int selection) {
    switch (selection) {
        case COL_NAME:
            return gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry")));
        case COL_GEN:
            return gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry")));
        default:
            return "false";
    }
}

series * tw_to_struct(GtkTreeModel *model, GtkTreeIter *iter){
    gchararray status, watched, name, last_episode, genre;
    gint n_episodes, n_seasons, year;

    gtk_tree_model_get(model, iter, COL_NAME, &name, -1);
    gtk_tree_model_get(model, iter, COL_LAST_EP, &last_episode, -1);
    gtk_tree_model_get(model, iter, COL_N_EP, &n_episodes, -1);
    gtk_tree_model_get(model, iter, COL_N_S, &n_seasons, -1);
    gtk_tree_model_get(model, iter, COL_YEAR, &year, -1);
    gtk_tree_model_get(model, iter, COL_GEN, &genre, -1);
    gtk_tree_model_get(model, iter, COL_STAT, &status, -1);
    gtk_tree_model_get(model, iter, COL_WATCHED, &watched, -1);

    return data_to_struct(name, last_episode, n_episodes, n_seasons, year, genre, status, watched);
}

series * grid_to_struct(GtkGrid *data_grid){
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

    cout << series_name << endl;
    cout << series_last_ep << endl;
    cout << series_num_ep << endl;
    cout << series_num_seas << endl;
    cout << series_year << endl;
    cout << genre_string << endl;
    cout << status_string << endl;
    cout << watched_string << endl;
    cout << "**********" << endl;

    series *s = new series;
    s = data_to_struct(series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string,
                               status_string, watched_string);

    cout << "test contenuto s->name in grid_to_struct dopo data_to struct: " << s->name << endl;

    return s;
}
