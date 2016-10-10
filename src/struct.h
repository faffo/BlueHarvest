//
// Created by faffo on 26/01/16.
//
/**
 * @file struct.h file di header contenente le strutture base del progetto. Definisce anche alcune variabili globali.
 * Viene definito qui anche la macro di tracing per il debug.
 */
#include <libmng_types.h>
#include <gtk/gtk.h>

#ifdef DEBUG
#define DEB(a) {a; }
#else
#define DEB(a)
#endif
/**
 * Lunghezza massima del testo.
 */
const int LENGTH = 50;
/**
 * Percorso file contenente il percorso dell'ultimo file aperto
 */
const char DEFAULT_FNAME[] = "../out/default.conf";
/**
 * Percorso contenente il file .glade per le finestre di dialogo
 */
const char BUILDER_PATH_DIALOG[] = "../resources/glade/dialog.glade";
/**
 * Percorso contenente il file .glade per la finestra main
 */
const char BUILDER_PATH_MAIN[] = "../resources/glade/main.glade";

/*
enum t_status {
    ongoing = 0,
    concluded
};
 */
/*enum t_genre {
    horror = 0,
    fantasy,
    political,
    scifi,
    sitcom,
    soap,
    thriller
};
*/
/**
 * Enumera le colonne utilizzate dalla tree view di gtk per la visualizzazione delle informazioni sulla serie
 */
enum tm_columns {
    COL_NAME = 0, /**<Colonna 0 del nome*/
    COL_LAST_EP, /**<Colonna 1 dell'ultimo episodio*/
    COL_N_EP, /**<Colonna 2 del numero episodi*/
    COL_N_S, /**<Colonna 3 del numero stagioni*/
    COL_YEAR, /**<Colonna 4 dell'anno*/
    COL_GEN, /**<Colonna 5 del genere*/
    COL_STAT, /**<Colonna 6 dello stato*/
    COL_WATCHED /**<Colonna 7 del visto sì/no*/
};
/**
 * Enumerazione della modalità di modifica di una funzione
 */
enum edit_mode {
    DEL_MODE = 0, /**<Definita uguale a 0 dall'enum, più facile da ricordare*/
    EDIT_MODE /**<Definita uguale a 1 dall'enum. Più facilmente memorizzabile*/
};

/**
 * Struttura base per la creazione delle serie tv
 */
struct series {
    /**
     * Il nome della serie tv.
     */
    char name[LENGTH];
    /**
     * L'ultimo episodio andato in onda (si suggerisce un formato del tipo 01x03 (stagione numero x episodio numero)
     */
    char last_episode[LENGTH];
    /**
     * Il numero di episodi fin qui andati in onda
     */
    gint n_episodes;
    /**
     * Il numero di stagioni uscite ad oggi
     */
    gint n_seasons;
    /**
     * L'anno di uscita della serie
     */
    gint year;
    /**
     * Il genere della serie
     */
    char genre[LENGTH];
    /**
     * Variabile che indica se la serie è conclusa o meno boolean
     */
    bool status;
    /**
     * Variabile che indica se la serie è stata vista o meno boolean
     */
    bool watched;
};

/**
 * Struttura di supporto per passare le informazioni nei foreach di gtk e g_slist
 */
struct n_search_gtk {
    char *name; /**<Il nome della serie da ricercare */
    int mode; /**<La modalità di ricerca (permette di usare meno codice*/
    series *edited; /**<La serie modificata che sostituirà al ricercata*/
};
/**
* Struttura di supporto per passare le informazioni nei foreach di g_slist
*/
struct search_data {
    char *text; /**<Testo da passare alla funzione in g_slist_foreach*/
    int mode; /**Modalità di esecuzione<*/
};
