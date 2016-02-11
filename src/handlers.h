//
// Created by faffo on 03/02/16.
//

#ifndef BLUEHARVEST_HANDLERS_H
#define BLUEHARVEST_HANDLERS_H

#endif //BLUEHARVEST_HANDLERS_H

extern "C" void handler_add_series(GtkButton *gtkButton, gpointer add_series);

extern "C" void handler_save(GtkButton *gtkButton, gpointer save_dialog_pointer);

extern "C" void handler_add(GtkMenuItem *menuItem, gpointer dialog_add);

extern "C" void handler_dialog_open(GtkMenuItem *menuItem, gpointer file_choser_open);

extern "C" void handler_dialog_save_as(GtkMenuItem *menuItem, gpointer file_chooser_save);

extern "C" void handler_open_file(GtkButton *gtkButton, gpointer load_dialog_pointer);

extern "C" void handler_dialog_delete_series(GtkMenuItem *menuItem, gpointer dialog_delete_series);