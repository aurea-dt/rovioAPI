//////////////////////////////////////////////////////////////////////////////
//	My_guiClass.h
// 	Mario Chirinos Colunga
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	26 Jun 2013
//------------------------------------------------------------------------------
// Notes:
//	GTK+ interface
//		
//////////////////////////////////////////////////////////////////////////////
#ifndef MY_GUICLASS_H
#define MY_GUICLASS_H
// your public header include
#include "ADT_GTK.h"
#include "ADT_rovioAPI.h"
#include "ADT_joystick.h"
// the declaration of your class...
//------------------------------------------------------------------------------
class T_GTKapp : public ADT_GTK
{
 private:
	static int joystickTimer_cb(void* data);

 public:

	ADT_joystick* joystick;

	GtkWidget *window;
	
	//main window items
	//GtkWidget *autocap_checkbutton;
	//GtkIconTheme *myTheme;
	
	//member functions
	T_GTKapp(const char *fileName);

	void guiInit();

	//const char* fileChooserDialog(vector<char*> filters);
	//void guiRefresh();

};
//------------------------------------------------------------------------------
extern T_GTKapp *GTKapp;
extern ADT_rovioAPI *rovio;
#endif
