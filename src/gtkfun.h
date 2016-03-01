//
// Created by faffo on 03/02/16.
//

//#include "struct.h"
#include "manipulation.h"

#ifndef BLUEHARVEST_GTKFUN_H
#define BLUEHARVEST_GTKFUN_H

#endif //BLUEHARVEST_GTKFUN_H

void refresh_treeview(series *s, gpointer plist);

series * tw_to_struct(GtkTreeModel *, GtkTreeIter *);

void tw_edit(GtkTreeSelection *sel, int mode);

series * get_sel(GtkTreeSelection *);

void dialog_edit(series *, GtkTreeModel *);

gboolean read_list(GtkTreeModel *model,
                   GtkTreePath *path,
                   GtkTreeIter *iter,
                   gpointer data);

const char * get_entry(int);