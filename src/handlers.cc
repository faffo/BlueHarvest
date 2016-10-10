//
// Created by faffo on 03/02/16.
//
/**
 * @file handlers.cc file contenente gli handlers per la cattura dei segnali dalla GUI in gtk.
 * Crea inoltre le due liste utilizzate dal programma.
 */
#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "handlers.h"
#include "gtkfun.h"
#include "loadsave.h"

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

extern "C" void handler_delete_widget(GtkButton *gtkButton, gpointer widget) {
    destroy_widget(GTK_WIDGET(widget));
}
/**
 * Handler per la cattura del segnale di salvataggio di un file.
 */
extern "C" void handler_save_file(GtkButton *gtkButton, gpointer p_fc_widget) {

    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(p_fc_widget);
    const char *fname = gtk_file_chooser_get_filename(fileChooser); /*Il nome del file da salvare*/

    if (!save(l_series, fname)) { /*Chiamata alla funzione save all'interno dell'if per verificarne esito*/
        char *objects[] = {(char *) "w_error_save", NULL};
        gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
        gtk_builder_connect_signals(builder, NULL);
    } else {
        destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_save")));
    }

}
/**
 * Handler per la cattura del segnale per fare il sort di una treeview.
 */
extern "C" void handler_sort(GtkMenuItem *menuItem, gpointer tree) {

    gtk_list_store_clear(GTK_LIST_STORE(tree));
    const gchar *label = gtk_menu_item_get_label(menuItem); /*Label identificativa del menu della UI*/
    int selection = sort_menu_conv(label); /*Valore convertito in int*/
    if (selection != -1) {
        l_series = sort(l_series, selection); /*Chiamata funzione di ordinamento su lista l_series*/

        g_slist_foreach(l_series, (GFunc) refresh_treeview,
                        (gpointer) GTK_LIST_STORE(tree)); /*Aggiornamento della visualizzazione grafica*/
    }
}
/**
 * Handler per la cattura del segnale per alternare tra la visualizzazione della ricerca name e genre.
 */
extern "C" void handler_show_search(GtkMenuItem *menuItem, gpointer p_header) {
    /*Nasconde la barra della ricerca (se da nascondere)*/
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_name")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry")), "");
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_genre")));
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry")), "");

    gtk_widget_show_all(GTK_WIDGET(p_header)); /*Visualizza barra ricerca*/
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
extern "C" void handler_search_name(GtkButton *button, gpointer pname) {
    gchar *objects[] = {(gchar *) "w_found", (gchar *) "list_found", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
    //GtkListStore *listSeries = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series")));
    GtkListStore *listFound = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_found"));

    char *name = (gchar *) gtk_entry_get_text(
            GTK_ENTRY(gtk_builder_get_object(builder, "s_name_entry"))); /*Nome da ricercare*/

    g_slist_foreach(l_series, (GFunc) sname, name); /*Ricerco nome su ogni nodo di l_series*/

    gtk_list_store_clear(listFound);
    //DEB(gtk_list_store_clear(listSeries));
    g_slist_foreach(l_found, (GFunc) refresh_treeview, listFound);  /*Visualizzo risultati*/
    //DEB(g_slist_foreach(l_series, (GFunc) refresh_treeview, listSeries));

    g_slist_free(l_found);  /*Libero memoria occupata da l_found*/
    l_found = NULL;

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_name")));
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_genre")));
}
/**
 * Handler per la cattura del segnale per la ricerca del genere tra quelle della lista principale.
 */
extern "C" void handler_search_genre(GtkButton *button, gpointer pgenre) {
    gchar *objects[] = {(gchar *) "w_found", (gchar *) "list_found", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
    //GtkListStore *listSeries = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series")));
    GtkListStore *listFound = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_found"));

    char *genre = (gchar *) gtk_entry_get_text(
            GTK_ENTRY(gtk_builder_get_object(builder, "s_genre_entry"))); /*Genere da ricercare*/

    g_slist_foreach(l_series, (GFunc) sgenre, genre); /*Ricerco genre in ogni nodo di l_series*/

    gtk_list_store_clear(listFound);
    //DEB(gtk_list_store_clear(listSeries));
    g_slist_foreach(l_found, (GFunc) refresh_treeview, listFound);
    //DEB(g_slist_foreach(l_series, (GFunc) refresh_treeview, listSeries)); /*Visualizzo risultati ricerca*/

    g_slist_free(l_found);      /*Libero memoria occupata da l_found*/
    l_found = NULL;

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_name")));
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "s_header_genre")));
}
/**
 * Handler per la cattura del segnale per l'eliminazione di una serie dalla lista. Cattura ed elimina. Versione aggiornata con main
 */
extern "C" void handler_delete(GtkButton *button, gpointer pnull) {
    series *s;  /*La lista da cancellare*/
    GtkWidget *w_found = GTK_WIDGET(gtk_builder_get_object(builder, "w_found"));

    if (GTK_IS_WIDGET(w_found) && gtk_widget_is_visible(
            w_found)) {      /*If per determinare se la chiamata avviene dalla finestra w_found o direttamente da menù del main*/
        DEB(cout << "funziona l'if per l'handler delete?" << endl);
        s = get_sel(gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object(builder,
                                                                                     "tw_found"))));   /*Copio in s i dati della riga selezionata (da w_found)*/
        gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "w_found")));
    } else {
        s = get_sel(gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object(builder,
                                                                                     "tw_series"))));  /*Copio in s i dati della riga selezionata (da w_main)*/
    }

    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));

    DEB(cout << "test:" << s->name << endl);

    if (s->name) {
        char *name = s->name;   /*Uso nome per effettuare confronto nella ricerca*/
        del_list(l_series, search_list(l_series,
                                       name));    /*Chiamo del_list per cancellare da l_series il nodo trovato da search_list in base al nome fornito*/

        gtk_list_store_clear(listStore);
        g_slist_foreach(l_series, (GFunc) refresh_treeview,
                        (gpointer) listStore);      /*Refresh visualizzazione grafica*/
    }
    destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_dialog_delete")));

}
/**
 * Handler per la cattura del segnale per l'eliminazione di una serie dalla lista. Cattura ed elimina. Versione deprecata.
 */
extern "C" void handler_delete_old(GtkButton *button, gpointer pnull) {


    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object(builder, "tw_found")));

    //tw_edit(sel, DEL_MODE);
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    series *s = get_sel(sel);
    if (s->name) {
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
 * Handler per la cattura del segnale per la modifica di una serie della lista. Versione aggiornata con edit da main
 */
extern "C" void handler_edit(GtkButton *button, gpointer pnull) {
    series *s;  /*La serie da editare*/
    GtkWidget *w_found = GTK_WIDGET(gtk_builder_get_object(builder, "w_found"));

    if (GTK_IS_WIDGET(w_found) && gtk_widget_is_visible(
            w_found)) {  /*If per determinare se la chiamata avviene dalla finestra w_found o direttamente da menù del main*/
        s = get_sel(gtk_tree_view_get_selection(
                GTK_TREE_VIEW(gtk_builder_get_object(builder, "tw_found"))));   /*Prendo dati serie da w_found*/
    } else {
        s = get_sel(gtk_tree_view_get_selection(
                GTK_TREE_VIEW(gtk_builder_get_object(builder, "tw_series"))));  /*Prendo dati serie da w_main*/
    }
    if (s->name) {
        dialog_edit(s);     /*Chiamo dialog edit con dati forniti*/
    }
}
/**
 * Handler per la cattura del segnale per la modifica di una serie della lista. Versione deprecata.
 */
extern "C" void handler_edit_old(GtkButton *button, gpointer ptw_found) {
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptw_found));
    series *s = get_sel(sel);
    if (s->name) {
        dialog_edit(s);
    }
}
/**
 * Handler per la cattura del segnale che finalizza la modifica della serie. Deprecato.
 */
extern "C" void handler_edit_confirm_old(GtkButton *button, gpointer edit_series) {

    DEB(cout << "DENTRO HANDLER_EDIT_CONFIRM" << endl);
    //Inizializzo treeview found e series oltre che la selezione
    GtkTreeView *treeViewfound = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tw_found"));
    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    GtkTreeSelection *sel = gtk_tree_view_get_selection(treeViewfound);
    //Estrappolo la serie dalla selezione
    series *s = get_sel(sel);
    char *name = s->name;
    //creo iter
    //GtkTreeIter *iter = NULL;
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
    GSList *tmp = l_series;
    g_slist_foreach(tmp, (GFunc) edit, search_data);
    gtk_list_store_clear(listStore);
    g_slist_foreach(tmp, (GFunc) refresh_treeview, (gpointer) listStore);
    l_series = tmp;

    //gtk_tree_selection_get_selected(sel, (GtkTreeModel **) treeViewfound, iter);
    //gtk_list_store_set(GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series")), iter);

    gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "w_edit")));
    gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "w_found")));

}
/**
 * Handler per la cattura del segnale che finalizza la modifica della serie. Versione aggiornata con main.
 */
extern "C" void handler_edit_confirm(GtkButton *button, gpointer edit_series) {

    GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
    series *s;      /*Vecchia serie*/
    series *edited = grid_to_struct((GtkGrid *) edit_series); /*Serie modificata*/
    if (edited->n_episodes != NULL && edited->n_seasons != NULL && edited->year != NULL) {   /*controllo dati*/
        GtkWidget *w_found = GTK_WIDGET(gtk_builder_get_object(builder, "w_found"));

        if (GTK_IS_WIDGET(w_found) && gtk_widget_is_visible(
                w_found)) {  /*If per determinare se la chiamata avviene dalla finestra w_found o direttamente da menù del main*/
            DEB(cout << "ha funzionato l'if in hander edit confirm?" << endl);
            GtkTreeView *treeViewfound = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tw_found"));

            s = get_sel(gtk_tree_view_get_selection(treeViewfound));   /*Serie selezionata da w_found*/

            gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "w_edit")));
            gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "w_found")));
        } else { ;
            GtkTreeView *treeViewseries = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tw_series"));

            s = get_sel(gtk_tree_view_get_selection(treeViewseries));   /*Serie selezionata da w_main*/

            gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "w_edit")));
        }

        DEB(cout << "test contenuto nome edited->name: " << edited->name << endl);

        //creo struct per passaggio info a funzione find
        n_search_gtk *search_data = new n_search_gtk;
        search_data->name = s->name;
        search_data->mode = EDIT_MODE;
        search_data->edited = edited;

        g_slist_foreach(l_series, (GFunc) edit, search_data);   /*Edito serie con dati corrispondenti*/
        gtk_list_store_clear(listStore);
        g_slist_foreach(l_series, (GFunc) refresh_treeview, (gpointer) listStore); /*Refresh lista serie UI*/
    }
}
/**
 * Handler per la cattura del segnale per l'aggiunta di una serie alla lista.
 */
extern "C" void handler_add_series(GtkButton *gtkButton, gpointer add_series) {
    GtkGrid *data_grid = (GtkGrid *) add_series;
    series *s = grid_to_struct(data_grid);  /*Popolo la struct series con i dati ottenuti dalla grid*/
    if (s->n_episodes != NULL && s->n_seasons != NULL && s->year != NULL) {      /*controllo dati*/
        //insert_series(l_series,series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string, status_string, watched_string);
        GtkListStore *listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series"));
        refresh_treeview(s, (gpointer) listStore);  /*Aggiungo la serie alla tree view*/

        l_series = g_slist_append(l_series, s);     /*Aggiungo la serie alla lista*/

        destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_add_series")));
    }
}
/**
 * Handler per la cattura del segnale per il salvataggio della lista corrente.
 * Il nome file è lo stesso dell'ultimo aperto e quindi quello in uso.
 */
extern "C" void handler_save(GtkButton *gtkButton, gpointer save_dialog_pointer) {
    char fname[256];
    read_defaultfn(fname);      /*Leggo il nome del file contenuto dentro il file di apertura di default*/

    if (!save(l_series, fname)) {     /*Verifico che il salvataggio sia andato a buon fine*/
        char *objects[] = {(char *) "w_error_save", NULL};
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

    if (!open_file(fname, listStore)) {   /*Verifico che apertura sia andata a buon fine*/
        gchar *objects[] = {(gchar *) "w_error_open", NULL};
        gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
        gtk_builder_connect_signals(builder, NULL);
    } else {
        destroy_widget(GTK_WIDGET(gtk_builder_get_object(builder, "w_open")));
    }
}
/**
 * Handler per la cattura del segnale per la creazione della finestra che permette l'aggiunta di una serie
 */
extern "C" void handler_add(GtkMenuItem *menuItem, gpointer dialog_add) {
    gchar *objects[] = {(gchar *) "w_add_series", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}
/**
 * Handler per la cattura del segnale per la creazione della finestra che permette l'apertura di un file
 */
extern "C" void handler_dialog_open(GtkMenuItem *menuItem, gpointer file_choser_open) {
    gchar *objects[] = {(gchar *) "w_open", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}
/**
 * Handler per la cattura del segnale per la creazione della finestra che permette il salvataggio con nome
 */
extern "C" void handler_dialog_save_as(GtkMenuItem *menuItem, gpointer file_chooser_save) {
    gchar *objects[] = {(gchar *) "w_save", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}
/**
 * Handler per la cattura del segnale per la creazione della finestra che permette la cancellazione di una serie
 */
extern "C" void handler_dialog_delete(GtkMenuItem *menuItem, gpointer dialog_delete_series) {
    gchar *objects[] = {(gchar *) "w_dialog_delete", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}
/**
 * Handler per la cattura del segnale che cancella la lista corrente e la treeview corrente.
 * Serve per la creazione di una nuova lista e di conseguenza anche di un nuovo file database
 */
extern "C" void handler_new(GtkMenuItem *menuItem, gpointer pliststore) {
    GtkListStore *listStore = GTK_LIST_STORE(pliststore);
    //g_slist_foreach(l_series, (GFunc)g_free, NULL);
    g_slist_free(l_series);
    l_series = NULL;

    gtk_list_store_clear(listStore);
}
/**
 * Handler per la creazione della finestra delle info sul programma
 */
extern "C" void handler_about(GtkMenuItem *menuItem, gpointer pnull) {
    gchar *objects[] = {(gchar *) "w_about", NULL};
    gtk_builder_add_objects_from_file(builder, BUILDER_PATH_DIALOG, objects, NULL);
    gtk_builder_connect_signals(builder, NULL);
}