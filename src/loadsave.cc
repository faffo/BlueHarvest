/**
 * @file loadsave.cc file contenente le funzioni collegate alle operazioni di apertura/salvataggio su file esterno.
 */

#include <iostream>
#include <fstream>
#include <glib.h>
#include "gtkfun.h"

using namespace std;

/**
 * Nome dell'ultimo file aperto/salvato. Serve come apertura di default del programma.
 * @param[in] s ::series da salvare
 * @param[in] f ofstream a cui inviare i dati per il salvataggio
 */
static void save_series(series *s, ofstream f) {
    //cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year << " " <<
    //s->genre << " " << s->status << " " << s->watched << endl;
    f << " " << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year <<
      " " <<
      s->genre << " " << s->status << " " << s->watched;
}

/**
 * Funzione per la stampa di una struttura per motivi di DEBUG. Outdatata.
 * @param[in] strut Struttura in ingresso da stampare (tipo ::series)
 * @param[in] data Pointer data a tipo GSList*. non usato.
 * @return void
 */
void print(gpointer strut, gpointer data) {
    DEB(cout << "********************" << endl;
                cout << "dentro print foreach" << endl);
    series *s = (series *) strut;
    DEB(cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " <<
             s->year << " " << s->genre << " " << s->status << " " << s->watched << endl);

}

/**
 * Funzione per il salvataggio della lista su file. Prende in ingresso una lista (l_series in questo caso)
 * e un nome file.
 * @param[in] l_series Lista di ::series in ingresso sa salvare.
 * @param[in] f_name Il nome con cui cui salvare il file.
 * @return Ritorna vero se andata a buon fine o falso se ha fallito.
 */
bool save(GSList *l_series, const char f_name[]) {

    DEB(cout << "dentro save" << endl);

    ofstream f(f_name); /*Creo output file stream sul nome file indicato in ingresso*/

    if (!f) {   /*Controllo apertura riuscita*/
        return false;
    } else {
        ofstream df(DEFAULT_FNAME); /*Copio nel default.conf il nome del file appena salvato*/
        df << f_name;
        df.close();
        DEB(g_slist_foreach(l_series, (GFunc) print, NULL));
        g_slist_foreach(l_series, (GFunc) save_series, (gpointer) &f);  /*Salvo il contenuto di l_series su fname*/

        f.close();  /*Chiudo l'output file stream*/
        f.clear();
        return true;
    }
}

/**
 * Funzione per il caricamento di una lista da file. Prende in ingresso un nome file e copia su @param l_series il contenuto
 * @param[in,out] l_series La GSList di ::series su cui copiare il contenuto del file.
 * @param[in] f_name Il nome del file da cui prendere i dati.
 * @return Ritorna vero se è andato a buon fine. Falso se ha fallito.
 */
bool load(GSList *&l_series, const char f_name[]) {
    DEB(cout << "dentro load: " << f_name << endl);
    ifstream f(f_name); /*creo l'input file stream*/
    if (f.is_open()) {  /*Controllo riuscita dell'apertura*/
        //g_slist_free_full(l_series, g_free); /*Vuoto l_series*/
        //l_series = NULL;
        if (l_series != NULL) {
            l_series = free_list(l_series);
        }

        while (!f.eof()) {  /*Itero fino all'end of file e copio le varie parti contenuto nel file nella struttura*/
            series *s = new series;
            f >> s->name;
            f >> s->last_episode;
            f >> s->n_episodes;
            f >> s->n_seasons;
            f >> s->year;
            f >> s->genre;
            f >> s->status;
            f >> s->watched;

            DEB(cout << s->name << endl;
                        cout << s->last_episode << endl;
                        cout << s->n_episodes << endl;
                        cout << s->n_seasons << endl;
                        cout << s->year << endl;
                        cout << s->genre << endl;
                        cout << s->status << endl;
                        cout << s->watched << endl);

            l_series = g_slist_append(l_series, s); /*Aggiungo ogni serie letta alla l_series*/
        }
        ofstream df(DEFAULT_FNAME); /*Salvo nome file name nel default.conf*/
        df << f_name;
        df.close();
        f.close();  /*Chiudo file*/
        return true;
    } else {
        return false;
    }
}

/**
 * Funzione ausiliaria per il caricamento automatico all'avvio dell'ultimo file aperto.
 * @param[in] fname Il nome del file. La funzione modifica direttamente il fname dato in infgresso.
 * @return void
 */
void read_defaultfn(char *fname) {
    ifstream f(DEFAULT_FNAME);  /*Apro input file stream di default.conf*/
    while (!f.eof()) {  /*Itero fino end of file*/
        f >> fname; /*Leggo nome file contenuto*/
        DEB(cout << "dentro while(!f.eof()): " << fname << endl)
    }
}

