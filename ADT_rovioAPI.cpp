////////////////////////////////////////////////////////////////////////////////
// ADT_rovioAPI.cpp
// 	Mario Chirinos Colunga
// 	Oscar Sanchez Sioridia
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	17 Dic 2010 - 26 Jun 2013
//------------------------------------------------------------------------------
// Notes:
//	Rovio Wowwee API
//	www.myrovio.com
//
////////////////////////////////////////////////////////////////////////////////
#include "ADT_rovioAPI.h"
#include <sstream>
//#include <stdarg.h>
//#include <stdio.h>
//#include <errno.h>
//#include <string.h>
//#include <sys/socket.h>
//#include <resolv.h>
//#include <errno.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <fcntl.h>
//------------------------------------------------------------------------------
ADT_rovioAPI::ADT_rovioAPI(const char *_uri)
{
	uri = _uri;
	cout << "rovio uri: " << uri << endl;
//	curl_global_init(CURL_GLOBAL_ALL);
	streaming = new rovioStreaming(uri.c_str());	
	speak = new rovioSpeak(uri.c_str());
}
////------------------------------------------------------------------------------
ADT_rovioAPI::~ADT_rovioAPI()
{

	delete speak;
	delete streaming;
}
//------------------------------------------------------------------------------
int ADT_rovioAPI::curlCallback(char *data, unsigned int size, unsigned int nmemb, void *userdata)
{
//	cout << "BEGIN----curlCallback---" << endl;
//	string str;
//	str.append(data, size * nmemb);  
//	cout << "data: " <<  str << endl;
//	cout << "END----curlCallback---" << endl;
   	return nmemb*size;
}
//------------------------------------------------------------------------------
const char* ADT_rovioAPI::getURI() const
{
	return uri.c_str();
}
//------------------------------------------------------------------------------
int ADT_rovioAPI::sendCommand(const char *command) const
{
	cout << "sendCommand(" << command << ")" << endl;
	CURL *curl_handle = curl_easy_init();
	if(curl_handle)
	{
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &curlCallback); 
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
		stringstream ss;
		string str;
		ss <<"http://" << uri << command << endl;
		getline(ss,str);
		curl_easy_setopt(curl_handle, CURLOPT_URL, str.c_str());
		CURLcode res = curl_easy_perform(curl_handle);
		if(res != CURLE_OK)
		{
			cerr << "curl error: " << curl_easy_strerror(res) << endl;
		}
		curl_easy_cleanup(curl_handle);
	}
 return 0;
}
//------------------------------------------------------------------------------
void ADT_rovioAPI::stop() const
{
	sendCommand("/rev.cgi?Cmd=nav&action=18&drive=0");
}
//------------------------------------------------------------------------------
void ADT_rovioAPI::setHome() const
{
	sendCommand("/rev.cgi?Cmd=nav&action=14");
}
//------------------------------------------------------------------------------
void ADT_rovioAPI::goHome() const
{
	sendCommand("/rev.cgi?Cmd=nav&action=13");
}
//------------------------------------------------------------------------------
void ADT_rovioAPI::move(double angle, unsigned int speed) const
{
	cout << "move(" << angle << ", " << speed << ")" << endl;
	int dvalue = 0;
	if(angle == 0)
	{
		dvalue = FORWARD;
	}
	if(angle == 180)
	{
		dvalue = BACKWARD;
	}
	if(angle == 270)
	{
		dvalue = LEFT;
	}
	if(angle == 90)
	{
		dvalue = RIGHT;
	}
	if(angle > 0 && angle < 90)
	{
		dvalue = DIAGFRIGHT;
	}
	if(angle > 90 && angle < 180)
	{
		dvalue = DIAGBRIGHT;
	}
	if(angle > 180 && angle < 270)
	{
		dvalue = DIAGBLEFT;
	}
	if(angle > 270 && angle < 360)
	{
		dvalue = DIAGFLEFT;
	}
	stringstream ss;
	ss << "/rev.cgi?Cmd=nav&action="<< MANUALDRIVE <<"&drive=" << dvalue << "&speed=" << speed;
	string str;
	getline(ss, str);
	sendCommand(str.c_str());
}
//------------------------------------------------------------------------------
void ADT_rovioAPI::rotate(double angle, unsigned int speed) const
{
	cout << "rotate(" << angle << ", " << speed << ")" << endl;
	if(angle==0)
		return;

	stringstream ss;
	if(angle<0)
		ss << "/rev.cgi?Cmd=nav&action=" << MANUALDRIVE << "&drive=" << RCCW << "&speed=" << speed;
	if(angle>0)
		ss << "/rev.cgi?Cmd=nav&action=" << MANUALDRIVE << "&drive=" << RCW << "&speed=" << speed;
	string str;
	getline(ss, str);
	sendCommand(str.c_str());
}
//------------------------------------------------------------------------------
void ADT_rovioAPI::headUp() const
{
	stringstream ss;
	ss << "/rev.cgi?Cmd=nav&action=" << MANUALDRIVE << "&drive=" << HEADUP;
	string str;
	getline(ss, str);
	sendCommand(str.c_str());
}
//------------------------------------------------------------------------------
void ADT_rovioAPI::headMiddle() const
{
	stringstream ss;
	ss << "/rev.cgi?Cmd=nav&action=" << MANUALDRIVE << "&drive=" << HEADMIDDLE;
	string str;
	getline(ss, str);
	sendCommand(str.c_str());
}
//------------------------------------------------------------------------------
void ADT_rovioAPI::headDown() const
{
	stringstream ss;
	ss << "/rev.cgi?Cmd=nav&action=" << MANUALDRIVE << "&drive=" << HEADDOWN;
	string str;
	getline(ss, str);
	sendCommand(str.c_str());
}
//------------------------------------------------------------------------------
void ADT_rovioAPI::light(int l) const
{
	stringstream ss;
	ss << "/rev.cgi?Cmd=nav&action=" << RESERVED << "&LIGHT=" << l;
	string str;
	getline(ss, str);
	sendCommand(str.c_str());
}
//------------------------------------------------------------------------------


