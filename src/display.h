//
// Created by faffo on 26/01/16.
//

#include <glib/gslist.h>
#include <iostream>

GSList seek(GSList *list, const char l_name[]);

void view_list(GSList *list, std::ostream &o);