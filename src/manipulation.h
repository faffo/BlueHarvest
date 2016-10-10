//
// Created by faffo on 26/01/16.
//
/**
 * @file manipulation.h file di header contenente di manipulation.cc
 */
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

GSList *sort(GSList *list, int picked);

int sort_menu_conv(const char *);

const gchararray bool2text(bool text);

void s_data(series *serie, gpointer pdata);

void sname(series *, gpointer);

void sgenre(series *, gpointer);

const char *itochar(gint);

void edit(gpointer, gpointer);

GSList *search_list(GSList *list, const char nome[]);

void del_list(GSList *&list, GSList *tmp);