//////////////////////////////////////////////////////////////////////////////
//	My_callbacks.cpp
// 	Mario Chirinos Colunga
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	26 Jun 2013
//------------------------------------------------------------------------------
// Notes:
//	Signals call backs		
//		
//////////////////////////////////////////////////////////////////////////////
#include "My_callbacks.h"
#include "My_guiClass.h"
#include <iostream>
#include <cmath>
using namespace std;
//------------------------------------------------------------------------------
int on_idle_callback(void *data)
{

 return true;
}
//------------------------------------------------------------------------------
void on_main_window_destroy(GtkObject *object, void* user_data)
{
        gtk_main_quit();
}
//------------------------------------------------------------------------------
void hide_on_window_delete_event(GtkObject *object, void* user_data)
{//cout<<"hide"<<endl;
	gtk_widget_hide_on_delete((GtkWidget*)object);
}
//------------------------------------------------------------------------------
void on_connect_button_clicked(unsigned int number, int value, void *userdata)
{
	rovio->streaming->connect();
}
//------------------------------------------------------------------------------
void axis_callback(unsigned int number, int value, void *userdata)
{
//	cout << ((ADT_joystick*)userdata)->getDevName() << " axis event #"<< number << " : " << value << endl;

}
//------------------------------------------------------------------------------
void button_callback(unsigned int number, int value, void *userdata)
{
	cout << ((ADT_joystick*)userdata)->getDevName() << " button event #"<< number << " : " << value << endl;
//	return;
	switch(number)
	{
		case 1:
			rovio->headDown();
			break;
		case 2:
			rovio->headMiddle();
			break;
		case 3:
			rovio->headUp();
			break;
		case 4:
			rovio->light(0);
			rovio->speak->speak("Light off");	
			break;
		case 5:
			rovio->light(1);
			rovio->speak->speak("Light on");
			break;
		default:
			break;
	}

}
//------------------------------------------------------------------------------

