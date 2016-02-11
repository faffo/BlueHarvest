//
// Created by faffo on 26/01/16.
//

#include <iostream>
#include <glib.h>
#include "struct.h"

using namespace std;



//series serie;


const gchararray status_int2text(int status) {
    if (status) return (gchararray const) "Yes";
    else return (gchararray const) "No";
}

const gchararray watched_int2text(int watched) {
    if (watched) return (gchararray const) "Yes";
    else return (gchararray const) "No";
}

const int yn_text2int(gchararray watched) {
    if (!strcmp(watched, "Yes")) return 1;
    else return 0;
}

const char *int2stat(int stat) {
    switch (stat) {
        case 0:
            return "Ongoing";
        case 1:
            return "Concluded";
    }
}

bool del(GSList *&list, GSList *tmp) {
    list = g_slist_remove_link(list, tmp);
    g_slist_free_1(tmp);
    return true;
}

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
    s->status = yn_text2int(status);
    s->watched = yn_text2int(watched);

    //cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year << " " <<
    //s->genre << " " << s->status << " " << s->watched << endl;

    return s;
}

static int intcmp(int a, int b) {
    if (a < b) return -1;
    if (a == b) return 0;
    return 1;
}

static int compare(gconstpointer first, gconstpointer second, gpointer sel) {
    series *a = (series *) first;
    series *b = (series *) second;
    int selection = GPOINTER_TO_INT(sel);

    switch (selection) {
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
    }
}

GSList *sort(GSList *list, int selection) {
    gpointer sel = (gpointer) selection;
    list = g_slist_sort_with_data(list, (GCompareDataFunc) compare, sel);
    return list;
}

int sort_menu_conv(const char *label) {
    if (!strcmp(label, "Name")) return COL_NAME;
    if (!strcmp(label, "Year")) return COL_YEAR;
    if (!strcmp(label, "Genre")) return COL_GEN;
    if (!strcmp(label, "Number of Episodes")) return COL_N_EP;
    if (!strcmp(label, "Watched")) return COL_WATCHED;
    if (!strcmp(label, "Status")) return COL_STAT;
    if (!strcmp(label, "Number of Seasons")) return COL_N_S;
}

int search_conv(const char *label) {
    if (!strcmp(label, "Name")) return COL_NAME;
    if (!strcmp(label, "Genre")) return COL_GEN;
}


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


void insert_series(GSList *&list, series *sin) {
    cout << "**************************" << endl;
    cout << "dentro insert_series" << endl;
    series *s = new series;
    s = sin;
    cout << s->name << " " << s->last_episode << " " << s->n_episodes << " " << s->n_seasons << " " << s->year << " " <<
    s->genre << " " << s->status << " " << s->watched << endl;

    list = g_slist_append(list, s);
}

GSList* search(GSList *list, GSList *l_found, const int selection, const char *label) {
    GSList *tmp = NULL;
    tmp = list;
    if(l_found!=NULL){
        g_slist_foreach(l_found, (GFunc)g_free, NULL);
        g_slist_free(l_found);
        l_found = NULL;
    }

    series *s = new series;
    while (tmp != NULL) {
        if (selection == COL_NAME) {
            if (!strcmp(((series *) tmp->data)->name, label)) {
                cout << "controllo valore tmp->data->name:   ";
                cout << ((series *) tmp->data)->name << endl;
                s = (series *) tmp->data;
                cout << s->name << endl;
                insert_series(l_found, s);
                g_slist_foreach(tmp, (GFunc)g_free, NULL);
                g_slist_free(tmp);
                cout << "test l_found name:  ";
                cout << ((series*)(l_found->data))->name << endl;
                return l_found;
            }
        } else if(selection == COL_GEN){
            if (!strcmp(((series *) tmp->data)->genre, label)){
                cout << "controllo valore tmp->data->genre:   ";
                cout << ((series *) tmp->data)->genre << endl;
                s = (series *) tmp->data;
                l_found = g_slist_append(l_found, s);
                cout << "test l_found genre:  ";
                cout << ((series*)(l_found->data))->genre << endl;
            }
        }
        tmp = g_slist_next(tmp);
    }
    g_slist_foreach(tmp, (GFunc)g_free, NULL);
    g_slist_free(tmp);
    return l_found;
}

/*
bool edit(GSList* &list, const char name[], GSList* tmp, int pick, int status){
    switch (pick){
        case 0:
            strcpy(((series*) tmp->data)->name, name);
            return true;
        default:
            ((series*)tmp->data)->status= static_cast<t_status>(status);
    }
}
*/
/*
void pure_char_string_conv(const char name[], char new_name[]) {
    stringstream name1;
    string s;
    char c;
    int i = 0;
    name1.write(name, LENGTH - 1);
    getline(name1, s);
    string dest;
    remove_copy(s.begin(), s.end(), back_inserter(dest), ' ');
    strcpy(new_name, dest.c_str());
    while (new_name[i]) {
        new_name[i] = tolower(new_name[i]);
        i++;
    };
}
 */