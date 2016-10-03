//
// Created by faffo on 26/01/16.
//

#include <iostream>
#include <glib.h>
#include "struct.h"

using namespace std;

//series serie;

extern GSList *l_found;

const gchararray status_int2text(int status) {
    if (status) return (gchararray const) "Yes";
    else return (gchararray const) "No";
}

const gchararray watched_int2text(int watched) {
    if (watched) return (gchararray const) "Yes";
    else return (gchararray const) "No";
}

const char* itochar(gint number){
    string str = to_string(number);
    const char* text = str.c_str();
    DEB(cout << "text in itochar: " << text << endl);
    return text;
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

gboolean gtk_del(GtkTreeModel *model,
                   GtkTreePath *path,
                   GtkTreeIter *iter,
                   gpointer pseries){
    //GSList** pList = (GSList **) plist;
    //GSList* &list = *pList;
    series *s = (series *) pseries;
    char *name_searched = s->name;
    gchararray name;

    gtk_tree_model_get(model, iter, COL_NAME, &name, -1);
    if(!strcmp(name_searched, name)){
        GtkListStore *listStore = GTK_LIST_STORE(model);
        gtk_list_store_remove(listStore, iter);
    }
    return FALSE;
}

gboolean find_tw(GtkTreeModel *model,
                GtkTreePath *path,
                GtkTreeIter *iter,
                gpointer psearch){
    n_search_gtk *search = (n_search_gtk *) psearch;
    char *name_searched = search->name;
    DEB(cout << "test nome in find_tw: " << name_searched << endl);
    int mode = search->mode;
    DEB(cout << "test mode in find_tw: " << search->mode << endl);
    gchararray name;
    GtkListStore *listStore = GTK_LIST_STORE(model);

    gtk_tree_model_get(model, iter, COL_NAME, &name, -1);
    if(!strcmp(name_searched, name)){
        switch (mode) {
            case DEL_MODE: {
                gtk_list_store_remove(listStore, iter);
                return true;
            }
            case EDIT_MODE: {
                DEB(cout << "testo ingresso in EDIT_MODE di edit_tw" << endl);
                series *edited = search->edited;
                DEB(cout << "test del contenuto di edited in EDIT_MODE dopo assegnamento prima di list_store_set" << endl);
                gtk_list_store_set(listStore, iter, 0, edited->name,
                                   1, edited->last_episode,
                                   2, edited->n_episodes,
                                   3, edited->n_seasons,
                                   4, edited->year,
                                   5, edited->genre,
                                   6, status_int2text(edited->status),
                                   7, watched_int2text(edited->watched),
                                   -1);
                return true;
            }
            default:;
        }
    }
    return FALSE;
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

GSList* search(GSList *list, const int selection, const char *label) {

}

void sname(series* serie, gpointer pname){
    const char* name =(const char *) pname;
    DEB(cout << "name in sname:" << name << endl;
    cout << "serie->name:" << serie->name << endl);

    if(strcasestr(serie->name, name)){
        l_found = g_slist_append(l_found, serie);
        DEB(cout << "found" << endl);
        series* testsearch = (series *) l_found->data;
        DEB(cout << "name in l_found" << testsearch->name << endl);
    }
}

void sgenre(series* serie, gpointer pgenre){
    const char* genre = (const char *) pgenre;
    if(strcasestr(serie->genre, genre)){
        l_found = g_slist_append(l_found, serie);
    }
}

void print_name_test(series* serie){
    DEB(cout << serie->name << endl);
}

void edit(gpointer pold, gpointer pedit){
    DEB(cout << "dentro edit in foreach" << endl);
    series* old = (series *) pold;
    n_search_gtk* data = (n_search_gtk *) pedit;
    series* edited = data->edited;
    DEB(cout << "old->name: " << old->name << endl;
    cout << "edited->name: " << edited->name << endl);
    if(!strcasecmp(old->name, data->name)){
        strcpy(old->name, edited->name);
        strcpy(old->last_episode, edited->last_episode);
        old->n_episodes = edited->n_episodes;
        old->n_seasons = edited->n_seasons;
        old->year = edited->year;
        strcpy(old->genre, edited->genre);
        old->status=edited->status;
        old->watched=edited->watched;
        return;
    }
}

void del(gpointer ptarget, gpointer pname){
    series * target = (series *) ptarget;
    char * name = (char *) pname;
    if(!strcasecmp(target->name, name)){

    }
}

GSList* search_list(GSList* list, const char nome[])
{
    GSList* tmp = list;
    while (tmp != NULL)  {
        if (!strcmp(((series*) tmp->data)->name, nome)){
            DEB(cout<<"trovato"<<endl);
            return tmp;
        }
        tmp=g_slist_next(tmp);
    }
    return NULL;
}

bool del_list(GSList* &list, GSList*tmp)
{
    list=g_slist_remove_link (list,tmp);
    g_slist_free_1 (tmp);
    return true;
}