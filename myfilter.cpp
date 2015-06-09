////////////////////////////////////////////////////////////////////////////////
// 	myfilter.cpp
// 	Mario Chirinos Colunga
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	26 Jun 2013
//------------------------------------------------------------------------------
// Notes:
//	Video filter example
//
////////////////////////////////////////////////////////////////////////////////
#include "myfilter.h"
#include <iostream>
using namespace std;
//------------------------------------------------------------------------------
void myFilter(unsigned char *data, unsigned int width, unsigned int height, unsigned int nChannels, void* userdata)
{
	int th = 128;
        unsigned int i = width*height;
	for(;i--;)//unsigned int i = 0; i<width*height; i++)
	{
		for(unsigned int c = 0; c < nChannels; c++)
			*data++=(*data < th ? 0 : 255);
	}
}

//------------------------------------------------------------------------------
