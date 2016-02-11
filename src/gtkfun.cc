//
// Created by faffo on 03/02/16.
//

#include <gtk/gtk.h>
#include "gtkfun.h"

using namespace std;

extern GtkBuilder *builder;
extern GSList *l_series;

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

gboolean read_list(GtkTreeModel *model,
                   GtkTreePath *path,
                   GtkTreeIter *iter,
                   gpointer data) {

    gchararray status, watched, name, last_episode, genre;
    gint n_episodes, n_seasons, year;
    //GSList** pList = (GSList **) list;
    //GSList* &l_series = *pList;

    gchar *tree_path_string;
    tree_path_string = gtk_tree_path_to_string(path);
    cout << tree_path_string << endl;

    //gchar* tree_path_string;
    gtk_tree_model_get(model, iter, COL_NAME, &name, -1);
    gtk_tree_model_get(model, iter, COL_LAST_EP, &last_episode, -1);
    gtk_tree_model_get(model, iter, COL_N_EP, &n_episodes, -1);
    gtk_tree_model_get(model, iter, COL_N_S, &n_seasons, -1);
    gtk_tree_model_get(model, iter, COL_YEAR, &year, -1);
    gtk_tree_model_get(model, iter, COL_GEN, &genre, -1);
    gtk_tree_model_get(model, iter, COL_STAT, &status, -1);
    gtk_tree_model_get(model, iter, COL_WATCHED, &watched, -1);

    series *s = data_to_struct(name, last_episode, n_episodes, n_seasons, year, genre, status, watched);
    //cout << "dopo data_to_struct dentro read_list"<< endl;
    //cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year << " " << s->genre << " " << s->status << " " << s->watched << endl;
    insert_series(l_series, s);
    //series * prova = (series *) l_series->data;
    //cout <<"valore tornato a read_list "<< prova->name << endl;


    return FALSE;
}

const char* get_entry(int selection){
    switch(selection){
        case COL_NAME: return gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry")));
        case COL_GEN: return gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry")));
        default: return "false";
    }
}