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
#include "gtkfun.h"

using namespace std;

/**
 * Builder dell'interfaccia grafica GTK per la creazione dell'UI del programma tramite GLADE
 */
GtkBuilder *builder;

/**
 * variabile per la selezione
 */
static int pick;

/**
 * Funzione main del progetto.
 * Inizializza l'ambiente grafico GTK, crea il builder su cui inserire le varie componenti gtk grafiche
 * e aggiunge le componenti essenziali al funzionamento della schermata principale.
 * 
 * Per la creazione delle varie componenti grafiche viene utilizzato GLADE.
 * L'intero programma carica per ogni schermata solo le componenti grafiche necessarie.
 * Il nome delle componenti da caricare viene inserito in un vettore di stringhe terminante nell'elemento NULL.
 * Tale vettore viene poi inserito nella funzione gtk_builder_add_objects_from_file per l'aggiunta all'interfaccia grafica.
 * Per modificare le componenti da caricare basta quindi modificare il vettore di stringhe.
 * Il nome di ogni componente indicato in questo vettore deve corrispondere a quello della componente definita in GLADE.
*/
int main(int argc, char *argv[]) {

    /**
     * Nome del file glade della main interface:
     */
    gchar file[] = "../src/resources/glade/main.glade";
    /**
     * Nome del file con apertura di default:
     */
    gtk_init(&argc, &argv);
    /**
     * Inizializzo builder e al momento della creazione gli faccio importare gli oggetti grafici creati con glade nel file: main.glade
     * Un altro modo di inizializzare il builder è il seguente:
     * Creo vettore di stringhe contenente i nomi delle componenti grafiche di GLADE da caricare
     * gchar *objects[] = {"combo_genre", "list_series", "w_main", "image1", "entry_completion_name", "entry_completion_genre", NULL};
     * Uso questa funzione passando la lista creata precedentemente:
     * gtk_builder_add_objects_from_file(builder, "../src/resources/glade/main.glade", objects, NULL);
     *
     */
    builder = gtk_builder_new_from_file(BUILDER_PATH_MAIN);
    /**
     * Se è salvato un file name come apertura di default apre il file
     */
    default_on_startup();

    gtk_builder_connect_signals(builder, NULL);
    gtk_main();
    return 0;
}

