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
/**
 * Lista principale di tipo semplice per la gestione dell'elenco delle serie.
 */
GSList *l_series = NULL;
/**
 * Lista secondaria di tipo semplice utilizzata per visualizzare delle serie accomunate
 * da determinati principi di ricerca.
 */
GSList *l_found = NULL;

/**
 * Handler per la cattura di un sengnale di distruzione di un widget.
 */
extern "C" void handler_delete_widget(GtkButton *gtkButton, gpointer widget) {
    destroy_widget(GTK_WIDGET(widget));
}
/**
 * Handler per la cattura del segnale di salvataggio di un file.
 */
extern "C" void handler_save_file(GtkButton *gtkButton, gpointer p_fc_widget) {

    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(p_fc_widget);
    const char *fname = gtk_file_chooser_get_filename(fileChooser);

    if(!save(l_series, fname)){
        char *objects[] = {"w_error_save", NULL};
        gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
        gtk_builder_connect_signals(builder, NULL);
    }
    else{
        destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_save")));
    }

}
/**
 * Handler per la cattura del segnale per fare il sort di una treeview.
 */
extern "C" void handler_sort(GtkMenuItem *menuItem, gpointer tree) {
    /*
    GtkTreeModel *treeModel = GTK_TREE_MODEL(tree);
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
/**
 * Handler per la cattura del segnale per alternare tra la visualizzazione della ricerca name e genre.
 */
extern "C" void handler_show_search(GtkMenuItem *menuItem, gpointer p_header){
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_name")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry")), "");
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_genre")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry")), "");

    GtkWidget *header_search = GTK_WIDGET(p_header);
    gtk_widget_show_all(header_search);
}
/**
 * Handler per la cattura del segnale per far partire la ricerca di una serie tra quelle della lista.
 */
/*
extern "C" void handler_search(GtkButton *gtkButton, gpointer plabel){

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
 */
/**
 * Handler per la cattura del segnale per la ricerca del nome tra quelle della lista principale.
 */
extern "C" void handler_search_name(GtkButton *button, gpointer pname){
    gchar *objects[] = {"w_found", "list_found", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
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
/**
 * Handler per la cattura del segnale per la ricerca del genere tra quelle della lista principale.
 */
extern "C" void handler_search_genre(GtkButton *button, gpointer pgenre){
    gchar *objects[] = {"w_found", "list_found", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);

    g_slist_foreach(l_found, (GFunc)g_free, NULL);
    g_slist_free(l_found);
    l_found = NULL;

    const gchar* s_genre = gtk_entry_get_text(GTK_ENTRY(pgenre));
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_found"));
    DEB(cout << s_genre << endl);

    g_slist_foreach(l_series, (GFunc) sgenre, (gpointer) s_genre);

    g_slist_foreach(l_found, (GFunc) refresh_treeview, (gpointer) listStore);

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_name")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry")), "");
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_genre")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry")), "");
}
/**
 * Handler per la cattura del segnale per l'eliminazione di una serie dalla lista.
 */
extern "C" void handler_delete(GtkButton *button, gpointer ptw_found) {
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptw_found));
    //tw_edit(sel, DEL_MODE);
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    series *s = get_sel(sel);
    if(s->name){
        n_search_gtk *search_data = new n_search_gtk;
        char *name = s->name;
        search_data->name = name;
        search_data->mode = DEL_MODE;
        DEB(cout << "test search_data-> mode in handler_delete: " << search_data->mode << endl);

        //gtk_tree_model_foreach(treeModel, find_tw, (gpointer) search_data);
        //gtk_tree_model_foreach(treeModel, gtk_del, (gpointer) search_data);

        del_list(l_series, search_list(l_series, name));

        gtk_list_store_clear(listStore);
        g_slist_foreach(l_series, (GFunc) refresh_treeview, (gpointer) listStore);

        destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_found")));
    }

}
/**
 * Handler per la cattura del segnale per la modifica di una serie della lista.
 */
extern "C" void handler_edit(GtkButton *button, gpointer ptw_found){
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptw_found));
    series *s = get_sel(sel);
    if(s->name) {
        dialog_edit(s);
    }
}
/**
 * Handler per la cattura del segnale che finalizza la modifica della serie.
 */
extern "C" void handler_edit_confirm(GtkButton *button, gpointer edit_series){
    DEB(cout << "DENTRO HANDLER_EDIT_CONFIRM" << endl);
    //Inizializzo treeview found e series oltre che la selezione
    GtkTreeView *treeViewfound = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tw_found"));
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

    DEB(cout << "test contenuto nome edited->name: " << edited->name << endl);

    //creo struct per passaggio info a funzione find
    n_search_gtk *search_data = new n_search_gtk;
    search_data->name = name;
    search_data->mode = EDIT_MODE;
    search_data->edited = edited;

    //eseguo treeview foreach
    //gtk_tree_model_foreach(treeModel, find_tw, (gpointer) search_data);



    //g_slist_foreach(l_series, (GFunc) print, NULL);
    g_slist_foreach(l_series, (GFunc) edit, search_data);
    //g_slist_foreach(l_series, (GFunc) print, NULL);
    //cout << "testo se ho sostituito s con edited: " << test->name << endl;
    //cout << "posizione elemente ricercato index: " << index <<endl;
    gtk_list_store_clear(listStore);
    g_slist_foreach(l_series, (GFunc) refresh_treeview, (gpointer) listStore);

    //gtk_tree_selection_get_selected(sel, (GtkTreeModel **) treeViewfound, iter);
    //gtk_list_store_set(GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series")), iter);

    gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "w_edit")));
    gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "w_found")));
}
/**
 * Handler per la cattura del segnale per l'aggiunta di una serie alla lista.
 */
extern "C" void handler_add_series(GtkButton *gtkButton, gpointer add_series) {
    GtkGrid *data_grid = (GtkGrid *) add_series;
    series *s = grid_to_struct(data_grid);
    //insert_series(l_series,series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string, status_string, watched_string);
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    refresh_treeview(s, (gpointer) listStore);

    l_series = g_slist_append(l_series, s);

    destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_add_series")));
}
/**
 * Handler per la cattura del segnale per il salvataggio della lista corrente.
 * Il nome file è lo stesso dell'ultimo aperto e quindi quello in uso.
 */
extern "C" void handler_save(GtkButton *gtkButton, gpointer save_dialog_pointer) {
    char fname[256];
    read_defaultfn(fname);

    if(!save(l_series, fname)){
        char *objects[] = {"w_error_save", NULL};
        gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
        gtk_builder_connect_signals(builder, NULL);
    }
}
/**
 * Handler per la cattura del segnale per l'apertura di un file
 */
extern "C" void handler_open_file(GtkButton *gtkButton, gpointer p_fc_widget) {
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    gtk_list_store_clear(listStore);

    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(p_fc_widget);
    const char *fname = gtk_file_chooser_get_filename(fileChooser);

    if(!open_file(fname, listStore)){
        gchar *objects[] = {"w_error_open", NULL};
        gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
        gtk_builder_connect_signals(builder, NULL);
    }
    else {
        destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_open")));
    }
}
/**
 * Handler per la cattura del segnale per la creazione della finestra che permette l'aggiunta di una serie
 */
extern "C" void handler_add(GtkMenuItem *menuItem, gpointer dialog_add) {
    gchar *objects[] = {"w_add_series", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}
/**
 * Handler per la cattura del segnale per la creazione della finestra che permette l'apertura di un file
 */
extern "C" void handler_dialog_open(GtkMenuItem *menuItem, gpointer file_choser_open) {
    gchar *objects[] = {"w_open", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}
/**
 * Handler per la cattura del segnale per la creazione della finestra che permette il salvataggio con nome
 */
extern "C" void handler_dialog_save_as(GtkMenuItem *menuItem, gpointer file_chooser_save) {
    gchar *objects[] = {"w_save", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}
/**
 * Handler per la cattura del segnale per la creazione della finestra che permette la cancellazione di una serie
 */
extern "C" void handler_dialog_delete_series(GtkMenuItem *menuItem, gpointer dialog_delete_series) {

}
/**
 * Handler per la cattura del segnale che cancella la lista corrente e la treeview corrente.
 * Serve per la creazione di una nuova lista e di conseguenza anche di un nuovo file database
 */
extern "C" void handler_new(GtkMenuItem *menuItem, gpointer pliststore){
    GtkListStore *listStore = GTK_LIST_STORE(pliststore);
    g_slist_foreach(l_series, (GFunc)g_free, NULL);
    g_slist_free(l_series);
    l_series = NULL;

    gtk_list_store_clear(listStore);

}