//
// Created by faffo on 03/02/16.
//
/**
 * @file gtkfun.cc file contenente funzioni volte al funzionamento della GUI in gtk.
 */
#include <gtk/gtk.h>
#include <stdlib.h>
#include "gtkfun.h"
#include "loadsave.h"

using namespace std;

extern GtkBuilder *builder;
extern GSList *l_series;
extern GSList *l_found;


void destroy_widget(GtkWidget *widget){
    gtk_widget_destroy(widget);
}

/**
 * Funzione che permette la rappresentazione grafica della lista di serie in una gtk_treeview.
 * I paramentri di entrata sono i nodi della lista l_series navigata tramite g_slist_foreach()
 * Ognuno di essi è quindi una serie:
 * @param[in] s serie contenuta nel nodo attraversato tipo ::series
 * @param[in] plist è un puntatore che punta invece al listStore di gtk per la rappresentazione grafica.
 * Esso puà essere sia il principale list_series sia l'ausiliario list_found
 */
void refresh_treeview(series *s, gpointer plist) {
    DEB(cout << "dentro refresh_treeview" << endl;
                  cout << s->name << endl);
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
                       6, bool2text(s->status),
                       7, bool2text(s->watched),
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
/**
 * Funzione per estrappolare i dati inerenti ad una riga selezionata nella treeView (e quindi una serie)
 * @param[in] sel fornisce selezione di gtk_tree_selection
 * La funzione ritorna una serie, dopo che i dati sono stati trasformati in struct da tw_to_struct
 * @return Ritorna tipo ::series dato da tw_to_struct
 */
series * get_sel(GtkTreeSelection *sel){
    GtkTreeModel *model_sel;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(sel, &model_sel, &iter)) {
        return tw_to_struct(model_sel, &iter);
    }
}
/**
 * Funzione che crea graficamente una finestra di dialogo contenente i dati della
 * serie in ingresso per permetterne la modifica
 * @param[in] s struct ::series passata alla funzione
 */
void dialog_edit(series *s){
    gchar* objects[] = {"w_edit", NULL};
    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/dialog.glade", objects, NULL);
    gtk_builder_connect_signals(builder, NULL);

    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_name"))), s->name);
    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_last_ep"))), s->last_episode);
    //convert int to text n_ep
    char n_ep_text[] = "";
    DEB(cout << "contenuto n_ep_text 1:" << n_ep_text << endl);
    strcpy(n_ep_text, itochar(s->n_episodes));
    DEB(cout << "contenuto n_ep_text 2:" << n_ep_text << endl);
    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_n_ep"))), n_ep_text);
    //convert int to text n_seasons
    char n_seas_text[] = "";
    DEB(cout << "contenuto n_seas_text 1:" << n_seas_text << endl);
    strcpy(n_seas_text, itochar(s->n_seasons));
    DEB(cout << "contenuto n_seas_text 2:" << n_seas_text << endl);
    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_n_seas"))), n_seas_text);
    //convert int to text year
    char year_text[] = "";
    DEB(cout << "contenuto year_text 1:" << year_text << endl);
    strcpy(year_text, itochar(s->year));
    DEB(cout << "contenuto year_text 2:" << year_text << endl)
    gtk_entry_set_text(GTK_ENTRY((gtk_builder_get_object(builder, "edit_entry_year"))), year_text);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX((gtk_builder_get_object(builder, "edit_combo_genre"))), s->genre);

    if(s->status){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "edit_checkb_status")), true);
    }
    if(s->watched){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "edit_checkb_watched")), true);
    }

}

/*
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
*/
/**
 * Funzione che ottiene il valore che si intende ricercare. Deprecata.
 * @param selection valore intero passato per definire quale colonna leggere
 * @return ritorna valore letto
 */
/*
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
*/
/**
 * Funzione per la traduzione di una treeview popolata in una struct di tipo series.
 * @param[in] model è il treemodel della treeview
 * @param[in] iter è il puntatore alla riga della treeview
 * @return ritorna i dati ottenuti dopo essere stati copiati in una struct ::series
 */
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
/**
 * Funzione per l'ottenimento di dati da una grid (in questo caso quella di edit)
 * per la creazione di una struct series.
 * @param[in] data_grid gtk_grid contenente i vari campi con i valori da utilizzare
 * @return Restituisce una struct s ::series con i dati ottenuti
 */
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

    DEB(cout << series_name << endl;
    cout << series_last_ep << endl;
    cout << series_num_ep << endl;
    cout << series_num_seas << endl;
    cout << series_year << endl;
    cout << genre_string << endl;
    cout << status_string << endl;
    cout << watched_string << endl;
    cout << "**********" << endl);

    series *s = new series;
    s = data_to_struct(series_name, series_last_ep, series_num_ep, series_num_seas, series_year, genre_string,
                               status_string, watched_string);

    DEB(cout << "test contenuto s->name in grid_to_struct dopo data_to struct: " << s->name << endl);

    return s;
}
/**
 * Funzione che esegue operazioni base all'avvio dell'applicazione.
 * Separata dal main per ridurre complessità dipendenze viene utilizzata per caricare l'ultimo file su cui stava lavorando
 * @return Void
 */
void default_on_startup(){
    char fname[256];
    read_defaultfn(fname);
    open_file(fname, GTK_LIST_STORE(gtk_builder_get_object(builder, "list_series")));
}
/**
 * Funzione che apre un file tramite il nome passato
 * @param[in] fname Nome assoluto del file
 * @param[in] ListStore gtk_list_store in cui salvare i dati letti
 * @return Ritorna vero se la lettura è andata a buon fine falso se non è stato possibile aprire file
 */
bool open_file(const char* fname, GtkListStore *listStore){
    if(!load(l_series, fname)){
        return false;
    };
    g_slist_foreach(l_series, (GFunc) refresh_treeview, (gpointer) listStore);
    return true;
}

