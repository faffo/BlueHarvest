//
// Created by faffo on 26/01/16.
//

#include <iostream>
#include <cstring>
#include <gtk/gtk.h>
#include <algorithm>
#include <sstream>
#include "struct.h"

using namespace std;

series *data_to_struct(const gchararray,
                       const gchararray,
                       const gint,
                       const gint,
                       const gint,
                       const gchararray,
                       gchararray,
                       gchararray);

void insert_series(GSList *&list, series* s);

GSList * sort(GSList *list, int picked);

int sort_menu_conv(const char *);

int search_conv(const char *label);

GSList* search(GSList *l_found, const int selection, const char *label);

int sort_name_conv(const char *name);

const gchararray status_int2text(int);

const gchararray watched_int2text(int);

bool edit(GSList *&list, const char name[], GSList *tmp, int pick, int status);

gboolean del(GtkTreeModel *model,
             GtkTreePath *path,
             GtkTreeIter *iter,
             gpointer data);

gboolean find_tw(GtkTreeModel *model,
             GtkTreePath *path,
             GtkTreeIter *iter,
             gpointer data);

void pure_char_string_conv(const char name[], char new_name[]);

void sname(series*, gpointer);

void sgenre(series*, gpointer);

void print_name_test(series*);

const char* itochar(gint);

void edit(gpointer, gpointer );