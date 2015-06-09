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
//#include <gtk/gtk.h>
#include "rovioSpeak.h"
using namespace std;
//------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
//	gtk_init (&argc, &argv);
	int i;
	rovioSpeak *speak = new rovioSpeak("192.168.1.65");
	speak->speak("Rovio online" );
	cout << "enter a number: " << endl;
	cin >> i; 
	speak->speak("Rovio online" );

//        gtk_main ();

 return 0;
}

//gcc -Wall -g -o tutorial main.c -export-dynamic `pkg-config --cflags --libs gtk+-2.0`
