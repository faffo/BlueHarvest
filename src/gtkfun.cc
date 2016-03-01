//
// Created by faffo on 03/02/16.
//

#include <gtk/gtk.h>
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

series * get_sel(GtkTreeSelection *sel){
    GtkTreeModel *model_sel;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(sel, &model_sel, &iter))
        return tw_to_struct(model_sel, &iter);
}

void dialog_edit(series *s, GtkTreeModel *treemodel){
    gchar* objects[] = {"w_edit_series", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/main.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
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