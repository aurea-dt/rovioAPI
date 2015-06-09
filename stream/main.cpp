//////////////////////////////////////////////////////////////////////////////
//	main.cpp
//	AUTOR
//	Aurea Desarrollo Tecnológico.
//	FECHA
//----------------------------------------------------------------------------
//	DESCRIPCIÓN
//	Notas:	
//		
//gcc -Wall -g main.c -o GTKapp -export-dynamic `pkg-config --cflags --libs gtk+-2.0`
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <gtk/gtk.h>
#include "rovioStreaming.h"
using namespace std;
//------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	gst_init (&argc, &argv);

	rovioStreaming *mySreaming = new rovioStreaming("192.168.1.65");
	mySreaming->connect();
	int i;
	cin >> i; 

	gtk_main ();

 return 0;
}

//gcc -Wall -g -o tutorial main.c -export-dynamic `pkg-config --cflags --libs gtk+-2.0`
