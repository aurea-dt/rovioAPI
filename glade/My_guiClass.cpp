//////////////////////////////////////////////////////////////////////////////
//	My_guiClass.cpp
// 	Mario Chirinos Colunga
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	26 Jun 2013
//------------------------------------------------------------------------------
// Notes:
//	GTK+ interface
//		
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cmath>
#include "My_guiClass.h"
#include "My_callbacks.h"
using namespace std;
T_GTKapp *GTKapp;
ADT_rovioAPI *rovio;
//------------------------------------------------------------------------------
T_GTKapp::T_GTKapp(const char *fileName)
{
        buildFromFile(fileName);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "main_window"));

        //add your code here...
     	//myTheme=  gtk_icon_theme_get_default();
	//gtk_icon_theme_append_search_path(myTheme, "icons/");  

	//main window items...


	g_object_unref (G_OBJECT (builder));
}
//------------------------------------------------------------------------------
//		Member functions
//------------------------------------------------------------------------------
void T_GTKapp::guiInit()
{
	joystick = new ADT_joystick();
	joystick->enumJoysticks();
	joystick->connect("/dev/input/js0");
//	joystick->setAxisCallback(&axis_callback);
	joystick->setButtonCallback(&button_callback);
	g_timeout_add(200, joystickTimer_cb, this);

}
//------------------------------------------------------------------------------
int T_GTKapp::joystickTimer_cb(void* data)
{
//	cout << "joystickTimer_cb" << endl;

	double x = 0;
	double y = 0;
	double r = 0;
//	cout << "SHRT_MAX: " << SHRT_MAX << endl;
	y = (double) ((T_GTKapp*)data)->joystick->getAxis(0)/SHRT_MAX;
	x = -1* (double)((T_GTKapp*)data)->joystick->getAxis(1)/SHRT_MAX;
	r = sqrt(x*x+y*y);

	if(r!=0)
	{
		double angle = acos(x/r)*180/M_PI;
		if(x<0 && y < 0 )
			angle = 360-angle;
		if(x>=0 && y < 0)
			angle = 360-angle;

		int speed = 10-r*10;
		cout << "x: " << x << ", y: " << y << ", r: " << r << endl;
		rovio->move(angle, speed);

	}
	if(((T_GTKapp*)data)->joystick->getAxis(3)!=0)
	{
		int speedRotation = (double)((T_GTKapp*)data)->joystick->getAxis(3)/SHRT_MAX*10;
		rovio->rotate(speedRotation, 10 - abs(speedRotation));
	}
	return true;
}
//------------------------------------------------------------------------------
/*void T_GTKapp::guiRefresh()
{	

}
//------------------------------------------------------------------------------
const char* T_GTKapp::fileChooserDialog(vector<char*> filters)
{
	char *filename = NULL;
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new ("Open Image",
					      (GtkWindow*)window,
					      GTK_FILE_CHOOSER_ACTION_OPEN,
					      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					      NULL);

	for(unsigned int i=0; i<filters.size(); i++)
	{
		GtkFileFilter *filter = gtk_file_filter_new ();
		gtk_file_filter_add_pattern (filter, filters[i]);
		gtk_file_chooser_add_filter((GtkFileChooser*)dialog, filter);
	}

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	}
	gtk_widget_destroy (dialog);
	return filename;
}
//------------------------------------------------------------------------------
*/
