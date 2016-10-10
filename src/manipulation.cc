//
// Created by faffo on 26/01/16.
//
/**
 * @file manipulation.cc file contenente le funzioni di manipolazione dati. Dalla conversione di valori alla modifica delle liste.
 */
#include <iostream>
#include <glib.h>
#include "struct.h"

using namespace std;

//series serie;

extern GSList *l_found;

/**
 * Funzione ausiliaria per trasformate i valori booleani contenuti in series (watched e status) in testo
 * @param[in] text testo in entrata
 * @return ritorna vero se testo = yes e falso se = no
 */
const gchararray bool2text(bool text) {
    if (text) return (gchararray const) "Yes";
    else return (gchararray const) "No";
}

/**
 * Funzione ausiliaria per trasformare un integer nel medesimo numero in formato char*.
 * @param[in] number numero intero in ingresso
 * @return Ritorna il numero in formato char*
 */
const char *itochar(gint number) {
    string str = to_string(number);
    const char *text = str.c_str(); /*Utilizzo funzione delle stringhe per la conversione in char da int*/
    DEB(cout << "text in itochar: " << text << endl);
    return text;
}

/**
 * Funzione ausiliaria per trasformare del testo in booleano (yes=true, no=false).
 * @param text Testo in ingresso da trasformare.
 * @return Ritorna true se testo = yes e false se testo = no
 */
const bool text2bool(gchararray text) {
    return !strcasecmp(text, "Yes");
}

/**
 * Funzione per cancellare una riga da un listStore data una serie in inngresso. Funzione non attualmente utilizzata.
 * Alternativa al modello proposto ora. Invece di cancellare elemento da lista e refreshare la treeview viene direttamente modificata la treeview.
 * @param[in] model TreeModel da ispezionare
 * @param[in] path Indicatore di percorso di gtk (non utilizzato ma necessario per utilizzare il foreach)
 * @param[in,out] iter Indicatore di linea attraversata
 * @param[in] pseries Serie da confrontare ed eliminare passata come gpointer generico.
 * @return Ritorna vero se la serie è stata trovata ed eliminata con successo. Falso altrimenti.
 */
gboolean gtk_del(GtkTreeModel *model,
                 GtkTreePath *path,
                 GtkTreeIter *iter,
                 gpointer pseries) {
    //GSList** pList = (GSList **) plist;
    //GSList* &list = *pList;
    series *s = (series *) pseries;
    char *name_searched = s->name;
    gchararray name;

    gtk_tree_model_get(model, iter, COL_NAME, &name, -1);   /*Trova l'iter a cui si trova il nome cercato*/
    if (!strcmp(name_searched, name)) {
        GtkListStore *listStore = GTK_LIST_STORE(model);
        gtk_list_store_remove(listStore, iter);
    }
    return FALSE;
}

/**
 * Funzione generica per la ricerca di una serie all'interno di una treeview. Accetta in ingresso più dati per determinare cosa si voglia fare con la serie ricercata
 * Supporta sia la cancellazione di una serie sia l'editing. Deprecata.
 * @param[in] model TreeModel su cui fare ricerca
 * @param[in] path Indicatore di percorso di gtk (non utilizzato ma necessario per utilizzare il foreach)
 * @param[in,out] iter Indicatore di linea attraversata
 * @param[in] psearch Struttura dati contenente la serie e la modalità in cui si vuol fare funzionare la funzione (EDIT_MODE, DEL_MODE)
 * @return Ritorna vero se la serie è stata trovata e l'operazione richiesta è stata effettuata con successo. Falso altrimenti.
 */
gboolean find_tw(GtkTreeModel *model,
                 GtkTreePath *path,
                 GtkTreeIter *iter,
                 gpointer psearch) {
    n_search_gtk *search = (n_search_gtk *) psearch;
    char *name_searched = search->name;
    DEB(cout << "test nome in find_tw: " << name_searched << endl);
    int mode = search->mode;
    DEB(cout << "test mode in find_tw: " << search->mode << endl);
    gchararray name;
    GtkListStore *listStore = GTK_LIST_STORE(model);

    gtk_tree_model_get(model, iter, COL_NAME, &name, -1);   /*Trova iter della serie il cui nome è stato ricercato*/
    if (!strcmp(name_searched, name)) {
        switch (mode) {
            case DEL_MODE: {
                gtk_list_store_remove(listStore, iter);
                return true;
            }
            case EDIT_MODE: {
                DEB(cout << "testo ingresso in EDIT_MODE di edit_tw" << endl);
                series *edited = search->edited;
                DEB(cout << "test del contenuto di edited in EDIT_MODE dopo assegnamento prima di list_store_set"
                         << endl);
                gtk_list_store_set(listStore, iter, 0, edited->name,
                                   1, edited->last_episode,
                                   2, edited->n_episodes,
                                   3, edited->n_seasons,
                                   4, edited->year,
                                   5, edited->genre,
                                   6, bool2text(edited->status),
                                   7, bool2text(edited->watched),
                                   -1);
                return true;
            }
            default:;
        }
    }
    return FALSE;
}

/**
 * Funzione ausiliaria per la creazione di una struct series da dei dati in ingresso.
 * @param[in] name Nome della serie
 * @param[in] last_episode Ultimo episodio
 * @param[in] n_episodes Numero episodi totali
 * @param[in] n_seasons Numero stagioni
 * @param[in] year Anno di uscita della serie
 * @param[in] gen Genere della serie
 * @param[in] status Flag in corso o terminata
 * @param[in] watched Flag in pari o meno
 * @return Ritora la struct ::series creata dai dati
 */
series *data_to_struct(const gchararray name,
                       const gchararray last_episode,
                       const gint n_episodes,
                       const gint n_seasons,
                       const gint year,
                       const gchararray gen,
                       const gchararray status,
                       const gchararray watched) {
    series *s = new series;

    strcpy(s->name, name);
    strcpy(s->last_episode, last_episode);
    s->n_episodes = n_episodes;
    s->n_seasons = n_seasons;
    s->year = year;
    strcpy(s->genre, gen);
    s->status = text2bool(status);
    s->watched = text2bool(watched);

    //cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year << " " <<
    //s->genre << " " << s->status << " " << s->watched << endl;

    return s;
}

/**
 * Funzione ausiliaria per la comparazione tra 2 interi. Mima il comportamento di strcmp
 * @param[in] a Primo intero da confrontare
 * @param[in] b Secondo intero da confrontare
 * @return Ritora 1 se a > b, 0 se a = b, -1 se a < b.
 */
static int intcmp(int a, int b) {
    if (a < b) return -1;
    if (a == b) return 0;
    return 1;
}

/**
 * Funzione ausiliaria confrontare 2 varioibili tra 2 serie.
 * @param[in] first Primo Serie da confrontare
 * @param[in] second Secondo elemento da confrontare
 * @param[in] sel Definisce quale variabile confrontare
 * @return Restituire il valore del confronto.
 */
static int compare(gconstpointer first, gconstpointer second, gpointer sel) {
    series *a = (series *) first;
    series *b = (series *) second;
    int selection = GPOINTER_TO_INT(sel);   /*Casto sel ad un int*/

    switch (selection) {    /*A seconda del tipo di comprazione richiesta da sel faccio comparazione*/
        case COL_NAME:
            return strcmp(a->name, b->name);
        case COL_YEAR:
            return intcmp(a->year, b->year);
        case COL_GEN:
            return strcmp(a->genre, b->genre);
        case COL_N_EP:
            return intcmp(a->n_episodes, b->n_episodes);
        case COL_WATCHED:
            return intcmp(a->watched, b->watched);
        case COL_STAT:
            return intcmp(a->status, b->status);
        case COL_N_S:
            return intcmp(a->n_seasons, b->n_seasons);
        default:
            return 0;
    }

}

/**
 * Funzione per l'ordinamento di una lista in ingresso
 * @param[in,out] list La lista da ordinare
 * @param[in] selection Il criterio su cui ordinare
 * @return Ritorna la lista ordinata
 */
GSList *sort(GSList *list, int selection) {
    gpointer sel = (gpointer) selection;
    list = g_slist_sort_with_data(list, (GCompareDataFunc) compare, sel);   /*Ordino utilizzando funzione di g_slist*/
    return list;
}

/**
 * Funzione ausiliaria per l'ordinamento di una lista in base alla label passata.
 * @param[in] label Definisce su cosa lavorare.
 * @return Ritorna un valore numerico corrispondente alla label.
 */
int sort_menu_conv(const char *label) {
    if (!strcmp(label, "Name")) return COL_NAME;
    if (!strcmp(label, "Year")) return COL_YEAR;
    if (!strcmp(label, "Genre")) return COL_GEN;
    if (!strcmp(label, "Number of Episodes")) return COL_N_EP;
    if (!strcmp(label, "Watched")) return COL_WATCHED;
    if (!strcmp(label, "Status")) return COL_STAT;
    if (!strcmp(label, "Number of Seasons")) return COL_N_S;
    else return -1;
}
/*
int search_conv(const char *label) {
    if (!strcmp(label, "Name")) return COL_NAME;
    if (!strcmp(label, "Genre")) return COL_GEN;
}
*/

/*
int sort_name_conv(const char *name) {
    if (!strcmp(name, "name") || !strcmp(name, "c_name")) return 1;
    if (!strcmp(name, "year") || !strcmp(name, "c_year")) return 2;
    if (!strcmp(name, "genre") || !strcmp(name, "c_genre")) return 3;
    if (!strcmp(name, "n_episodes") || !strcmp(name, "c_n_episodes")) return 4;
    if (!strcmp(name, "watched") || !strcmp(name, "c_watched")) return 5;
    if (!strcmp(name, "status") || !strcmp(name, "c_status")) return 6;
    if (!strcmp(name, "n_seasons") || !strcmp(name, "c_n_seasons")) return 7;
}
*/

/*
void insert_series(GSList *&list, series *sin) {
    DEB(cout << "**************************" << endl;
    cout << "dentro insert_series" << endl);
    series *s = new series;
    s = sin;
    DEB(cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year << " " <<
    s->genre << " " << s->status << " " << s->watched << endl);

    list = g_slist_append(list, s);
    DEB(cout << ((series *)list->data)->name << endl;
    cout << "**********" << endl);
}
*/
/**
 * Funzione per la copia di determinate serie cercate in una lista secondaria (l_found). Pensata per gestire molteplici criteri di ricerca
 * Deprecata. Al momento non funziona.
 * @param[in] serie ::series passata dalla funzione g_slist_foreach in cui questa funzione è utilizzata
 * @param[in] pdata puntatore a struct search_data contenente il testo da ricercare e il campo in cui ricercarlo (sotto forma di intero)
 */
void s_data(series *serie, gpointer pdata) {
    search_data *data = (search_data *) pdata;  /*Data per la ricerca*/
    DEB(cout << "name in s_data: " << data->text << endl;
                cout << "mode in s_data: " << data->mode << endl;)
    if (data->mode == COL_NAME) {
        DEB(cout << "name in sname:" << data->text << endl;
                    cout << "serie->name:" << serie->name << endl);

        if (strcasestr(serie->name, data->text)) {
            l_found = g_slist_append(l_found, serie);
            DEB(cout << "found" << endl);
            series *testsearch = (series *) l_found->data;
            DEB(cout << "name in l_found" << testsearch->name << endl);
        }
    }
    if (data->mode == COL_GEN) {
        if (strcasestr(serie->genre, data->text)) {
            l_found = g_slist_append(l_found, serie);
        }
    }
}

/**
 * Funzione per la copia di determinate serie cercate in una lista secondaria (l_found). Il criterio di ricerca è che la stringe di caratteri fornita
 * compaia all'interno del campo serie->name. case insensitive.
 * @param[in] serie La ::series in cui cercare una corrispondenza, passata da g_slist_foreach
 * @param[in] pname La serie di caratteri da ricercare.
 */
void sname(series *serie, gpointer pname) {
    char *name = (char *) pname;    /*Stringa da ricercare*/
    DEB(cout << "name in sname: " << name << endl);
    if (strcasestr(serie->name, name)) {  /*Ricerco la stringa all'interno del nome (case insensitive)*/
        l_found = g_slist_append(l_found, (gpointer) serie);    /*Se trovata aggiungo series a l_found*/
    }

}

/**
 * Funzione per la copia di determinate serie cercate in una lista secondaria (l_found). Il criterio di ricerca è che la stringe di caratteri fornita
 * compaia all'interno del campo serie->genre. case insensitive.
 * @param[in] serie La ::series in cui cercare una corrispondenza, passata da g_slist_foreach
 * @param[in] pgenre La serie di caratteri da ricercare.
 */
void sgenre(series *serie, gpointer pgenre) {
    char *genre = (char *) pgenre;  /*Stringa da ricercare*/
    DEB(cout << "genre in sname: " << genre << endl);
    if (strcasestr(serie->genre, genre)) {    /*Ricerco la stringa all'interno del genere (case insensitive)*/
        l_found = g_slist_append(l_found, (gpointer) serie);    /*Se trovata aggiungo series a l_found*/
    }

}
/*
void print_name_test(series* serie){
    DEB(cout << serie->name << endl);
}
*/
/**
 * Funzione per l'edit di una serie. In ingresso prende oltre alle 2 serie (la vecchia e la modificata) anche il nome usato per la ricerca delle serie.
 * Modifica la serie "old" il cui nome corrisponde con il parametro di ricerca passato.
 * @param[in] pold Serie da controllare e, se corrisponde, modificare. Passata da g_slist_foreach.
 * @param[in] pedit Struttura contenente la nuova serie e il nome per effettuare confronto.
 */
void edit(gpointer pold, gpointer pedit) {
    DEB(cout << "dentro edit in foreach" << endl);
    series *old = (series *) pold;  /*Vecchia serie da modificare*/
    n_search_gtk *data = (n_search_gtk *) pedit;    /*Dati contenenti nuova serie*/
    series *edited = data->edited;  /*Nuova serie da modificare*/
    DEB(cout << "old->name: " << old->name << endl;
                cout << "edited->name: " << edited->name << endl);
    if (!strcasecmp(old->name,
                    data->name)) { /*Ricerco la serie utilizzando nome passato nei dati e sostituisco i campi*/
        strcpy(old->name, edited->name);
        strcpy(old->last_episode, edited->last_episode);
        old->n_episodes = edited->n_episodes;
        old->n_seasons = edited->n_seasons;
        old->year = edited->year;
        strcpy(old->genre, edited->genre);
        old->status = edited->status;
        old->watched = edited->watched;
        return;
    }

}

/**
 * Funzione ausiliaria per la ricerca di una corrispondenza all'interno di una lista del nome di una serie.
 * Utilizzata per la cancellazione di un nodo dalla lista.
 * @param[in] list Lista su cui effettuare la ricerca.
 * @param[in] nome Nome da confrontare.
 * @param[in] Ritorna l'elemento trovato se confronto è positivo
 */
GSList *search_list(GSList *list, const char nome[]) {
    GSList *tmp = list;     /*Lista temporanea di scorrimento*/
    while (tmp != NULL) {
        if (!strcmp(((series *) tmp->data)->name, nome)) {    /*Verifico presenza*/
            DEB(cout << "trovato" << endl);
            return tmp;     /*Se trovato ritorno il puntatore al nodo della lista*/
        }
        tmp = g_slist_next(tmp);  /*Scorro tmp al prossimo nodo*/
    }
    return NULL;
}

/**
 * Funzione per la cancellazione di un elemento dalla lista inserita a parametro.
 * @param[in,out] list La lista da cui cancellare l'.
 * @param[in] L'elemento da cancellare.
 */
void del_list(GSList *&list, GSList *tmp) {
    list = g_slist_remove_link(list, tmp);
    g_slist_free_1(tmp);
}
