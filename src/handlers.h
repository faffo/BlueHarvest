//
// Created by faffo on 03/02/16.
//

#ifndef BLUEHARVEST_HANDLERS_H
#define BLUEHARVEST_HANDLERS_H

#endif //BLUEHARVEST_HANDLERS_H



extern "C" void handler_delete_widget(GtkButton *gtkButton, gpointer widget);

extern "C" void handler_save_file(GtkButton *gtkButton, gpointer p_fc_widget);

extern "C" void handler_sort(GtkMenuItem *menuItem, gpointer tree);

extern "C" void handler_show_search(GtkMenuItem *menuItem, gpointer p_header);

extern "C" void handler_search(GtkButton *gtkButton, gpointer plabel);

extern "C" void handler_search_name(GtkButton *button, gpointer pname);

extern "C" void handler_search_genre(GtkButton *button, gpointer pgenre);

extern "C" void handler_delete(GtkButton *button, gpointer ptw_found);

extern "C" void handler_edit(GtkButton *button, gpointer ptw_found);

extern "C" void handler_edit_confirm(GtkButton *button, gpointer edit_series);


extern "C" void handler_add_series(GtkButton *gtkButton, gpointer add_series);

extern "C" void handler_save(GtkButton *gtkButton, gpointer save_dialog_pointer);

extern "C" void handler_add(GtkMenuItem *menuItem, gpointer dialog_add);

extern "C" void handler_dialog_open(GtkMenuItem *menuItem, gpointer file_choser_open);

extern "C" void handler_dialog_save_as(GtkMenuItem *menuItem, gpointer file_chooser_save);

extern "C" void handler_open_file(GtkButton *gtkButton, gpointer load_dialog_pointer);

extern "C" void handler_dialog_delete_series(GtkMenuItem *menuItem, gpointer dialog_delete_series);