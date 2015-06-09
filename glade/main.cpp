//////////////////////////////////////////////////////////////////////////////
//	main.cpp
//	Mario Chirinos Colunga
//	Aurea Desarrollo Tecnológico.
//	2013/06/21
//----------------------------------------------------------------------------
//	Notas:	
//	
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <gtk/gtk.h>
#include "My_guiClass.h"
#include "My_callbacks.h"
using namespace std;
//------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	gst_init (&argc, &argv);

	GTKapp =new T_GTKapp("ui.glade");
        gtk_widget_show (GTKapp->window);
	rovio = new ADT_rovioAPI("192.168.1.69");
	rovio->speak->speak("Rovio online" );
	//g_idle_add(on_idle_callback, NULL);
	rovio->streaming->connect();
	GTKapp->guiInit();

        gtk_main ();

 return 0;
}

//gcc -Wall -g -o tutorial main.c -export-dynamic `pkg-config --cflags --libs gtk+-2.0`
