////////////////////////////////////////////////////////////////////////////////
//	My_callbacks.cpp
// 	Mario Chirinos Colunga
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	26 Jun 2013
//------------------------------------------------------------------------------
// Notes:
//	Signals call backs		
//
////////////////////////////////////////////////////////////////////////////////
#ifndef MY_CALLBACKS_H
#define MY_CALLBACKS_H

// your public header include
#include "ADT_GTK.h"
// the declaration of your class...
int on_idle_callback(void *data);
extern "C" void on_main_window_destroy	(GtkObject *object, void* user_data);
extern "C" void on_autocap_change	(GtkObject *object, void* user_data);
extern "C" void hide_on_window_delete_event(GtkObject *object, void* user_data); //oculatar la ventana en vez de destruirla
extern "C" void on_connect_button_clicked(unsigned int number, int value, void *userdata);
void axis_callback(unsigned int number, int value, void *userdata);
void button_callback(unsigned int number, int value, void *userdata);

#endif
