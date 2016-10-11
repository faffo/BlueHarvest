//
// Created by faffo on 03/02/16.
//
/**
 * @file gtkfun.h file di header di gtkfun.cc
 */

#include "manipulation.h"


void destroy_widget(GtkWidget *widget);

void refresh_treeview(series *s, gpointer plist);

series *tw_to_struct(GtkTreeModel *, GtkTreeIter *);

series *get_sel(GtkTreeSelection *);

void dialog_edit(series *);

series *grid_to_struct(GtkGrid *);

void default_on_startup();

bool open_file(const char *fname, GtkListStore *listStore);

void error_ins_data();

void error_ins_data_space();