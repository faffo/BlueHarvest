//
// Created by faffo on 03/02/16.
//
/**
 * @file handlers.h file di header di handlers.cc
 */
//#ifndef BLUEHARVEST_HANDLERS_H
//#define BLUEHARVEST_HANDLERS_H

//#endif //BLUEHARVEST_HANDLERS_H



extern "C" void handler_delete_widget(GtkButton *, gpointer);

extern "C" void handler_save_file(GtkButton *, gpointer);

extern "C" void handler_sort(GtkMenuItem *, gpointer);

extern "C" void handler_show_search(GtkMenuItem *, gpointer);

extern "C" void handler_search_name(GtkButton *, gpointer);

extern "C" void handler_search_genre(GtkButton *, gpointer);

extern "C" void handler_delete(GtkButton *, gpointer);

extern "C" void handler_edit(GtkButton *, gpointer);

extern "C" void handler_edit_confirm(GtkButton *, gpointer);


extern "C" void handler_add_series(GtkButton *, gpointer);

extern "C" void handler_save(GtkButton *, gpointer);

extern "C" void handler_add(GtkMenuItem *, gpointer);

extern "C" void handler_dialog_open(GtkMenuItem *, gpointer);

extern "C" void handler_dialog_save_as(GtkMenuItem *, gpointer);

extern "C" void handler_open_file(GtkButton *, gpointer);

extern "C" void handler_dialog_delete_series(GtkMenuItem *, gpointer);