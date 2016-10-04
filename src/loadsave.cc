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
 * @param strut Struttura in ingresso da stampare
 * @param data Pointer data a tipo GSList*. non usato.
 * @return void
 */
void print(gpointer strut, gpointer data) {
    DEB(cout << "********************" << endl;
                cout << "dentro print foreach" << endl);
    GSList *list = (GSList *) data;
    series *s = (series *) strut;
    DEB(cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " <<
             s->year << " " << s->genre << " " << s->status << " " << s->watched << endl);

}

/**
 * Funzione per il salvataggio della lista su file. Prende in ingresso una lista (l_series in questo caso)
 * e un nome file.
 * @param l_series Serie in ingresso sa salvare.
 * @param f_name Il nome con cui cui salvare il file.
 * @return Ritorna vero se andata a buon fine o falso se ha fallito.
 */
bool save(GSList *l_series, const char f_name[]) {

    DEB(cout << "dentro save" << endl);

/*
    if(ifstream(f_name)){
        if(!error_file_exist(f_name)) return false;
    }
*/
    ofstream f(f_name);

    if (!f) {
        return false;
    } else {
        ofstream df(DEFAULT_FNAME);
        df << f_name;
        g_slist_foreach(l_series, (GFunc) print, NULL);
        g_slist_foreach(l_series, (GFunc) save_series, (gpointer) &f);

        f.close();
        f.clear();
        return true;
    }
}

/**
 * Funzione per il caricamento di una lista da file. Prende in ingresso un nome file e copia su @param l_series il contenuto
 * @param l_series La GSList su cui copiare il contenuto del file.
 * @param f_name Il nome del file da cui prendere i dati.
 * @return Ritorna vero se è andato a buon fine. Falso se ha fallito.
 */
bool load(GSList *&l_series, const char f_name[]) {
    DEB(cout << "dentro load: " << f_name << endl);
    ifstream f(f_name);
    if (f.is_open()) {
        //g_slist_foreach(l_series, (GFunc) g_free, NULL);
        g_slist_free(l_series);
        l_series = NULL;
        while (!f.eof()) {
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

            l_series = g_slist_append(l_series, s);
        }
        ofstream df(DEFAULT_FNAME);
        df << f_name;
        return true;
    }
    return false;
}

/**
 * Funzione ausiliaria per il caricamento automatico all'avvio dell'ultimo file aperto.
 * @param fname Il nome del file. La funzione modifica direttamente il fname dato in infgresso.
 * @return void
 */
void read_defaultfn(char *fname) {
    ifstream f(DEFAULT_FNAME);
    while (!f.eof()) {
        f >> fname;
        DEB(cout << "dentro while(!f.eof()): " << fname << endl)
    }
}

