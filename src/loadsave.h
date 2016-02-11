//
// Created by faffo on 26/01/16.
//

#include <fstream>
#include <cstring>
#include <cstdlib>
#include <glib/gslist.h>

using namespace std;


bool save(GSList *s_list, const char f_name[]);

bool load(GSList *&list, const char f_name[]);

void print(gpointer, gpointer);