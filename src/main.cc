/**
 * @mainpage BlueHarvest
 * 
 * Programma gestionale di serie tv. Il programma consente di aggiungere modificare e cancellare e ordinare
 * una lista di serie tv catalogate secondo varie caratteristiche 
 * (nome, anno, numero episodi, genere, numero stagioni) oltre che segnarle come viste o concluse
 * La lista così prodotta può poi essere salvata e aperta in seguito.
 * 
 * @author Daniele Ruffini
 */

/**
 * @file file contenente il main del progetto
 */
 
#include <gtk/gtk.h>
#include <iostream>
//#include "manipulation.h"

using namespace std;

/**
 * Builder dell'interfaccia grafica GTK per la creazione dell'UI del programma
 */
GtkBuilder *builder;

/**
 * Lista per i dati di una serie
 */
//Probabilmente da cambiare. Sarebbe lista per dati di una serie, ma lo volevi fare a elem singoli
GSList *list_series;

/**
 * variabile per la selezione
 */
static int pick;

/**
 * Funzione main del progetto.
 * Inizializza l'ambiente grafico GTK, crea il builder su cui inserire le varie componenti gtk grafiche
 * e aggiunge le componenti essenziali al funzionamento della schermata principale.
 * 
 * Per la creazione delle varie
*/
int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gchar *objects[] = {"combo_genre", "list_series", "w_main", "image1", "entry_completion_name", "entry_completion_genre", NULL};

    gtk_builder_add_objects_from_file(builder, "../src/resources/glade/main.glade", objects, NULL);

    gtk_builder_connect_signals(builder, NULL);
    gtk_main();
    return 0;
}

