#include <iostream>
#include <fstream>
#include <glib.h>
#include "gtkfun.h"

using namespace std;

static void save_series(series *s, ofstream f) {
    //cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year << " " <<
    //s->genre << " " << s->status << " " << s->watched << endl;
    f << " " << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year <<
    " " <<
    s->genre << " " << s->status << " " << s->watched;
}

void print(gpointer strut, gpointer data) {
    cout << "********************" << endl;
    cout << "dentro print foreach" << endl;
    GSList *list = (GSList *) data;
    series *s = (series *) strut;
    cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " <<
    s->year << " " << s->genre << " " << s->status << " " << s->watched << endl;

}

bool save(GSList *l_series, const char f_name[]) {

    cout << "dentro save" << endl;

/*
    if(ifstream(f_name)){
        if(!error_file_exist(f_name)) return false;
    }
*/
    ofstream f(f_name);
/*
    if(!f){
        error_on_create_file(f_name);
        return false;
    }
*/

    //f.clear();
    g_slist_foreach(l_series, (GFunc) print, NULL);
    g_slist_foreach(l_series, (GFunc) save_series, (gpointer) &f);

    f.close();
    f.clear();
    return true;
}

bool load(GSList *&l_series, const char f_name[]) {
    ifstream f(f_name);
    if (f.is_open()) {
        g_slist_foreach(l_series, (GFunc)g_free, NULL);
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

            cout << s->name << endl;
            cout << s->last_episode << endl;
            cout << s->n_episodes << endl;
            cout << s->n_seasons << endl;
            cout << s->year << endl;
            cout << s->genre << endl;
            cout << s->status << endl;
            cout << s->watched << endl;

            l_series = g_slist_append(l_series, s);
        }
        //g_slist_foreach(l_series, (GFunc) refresh_treeview, NULL);
    }
    return true;
}

