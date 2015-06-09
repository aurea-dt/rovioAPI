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
#include "ADT_rovioAPI.h"
#include "myfilter.h"
using namespace std;
//------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	gst_init (&argc, &argv);

	ADT_rovioAPI *rovio = new ADT_rovioAPI("192.168.1.69");
	rovio->streaming->setVideoFilter(myFilter);
	rovio->streaming->startVideoFilter();
	rovio->streaming->connect();
	rovio->speak->speak("Rovio online" );
        gtk_main ();

 return 0;
}

