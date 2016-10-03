//
// Created by faffo on 26/01/16.
//

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
const char BUILDER_PATH_DIALOG[] = "../src/resources/glade/dialog.glade";
/**
 * Percorso contenente il file .glade per la finestra main
 */

const char BUILDER_PATH_MAIN[] = "../src/resources/glade/main.glade";

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
    COL_NAME = 0,
    COL_LAST_EP,
    COL_N_EP,
    COL_N_S,
    COL_YEAR,
    COL_GEN,
    COL_STAT,
    COL_WATCHED
};
/**
 * Enumerazione della modalità di modifica di una funzione
 */
enum edit_mode {
    DEL_MODE = 0,
    EDIT_MODE
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
     * Variabile che indica se la serie è conclusa o meno (1 o 0)
     */
    int status;
    /**
     * Variabile che indica se la serie è stata vista o meno (1 o 0)
     */
    int watched;
};

/**
 * Struttura di supporto per passare le informazioni nei foreach di gtk e g_slist
 */
struct n_search_gtk{
    char *name;
    int mode;
    series *edited;
};
