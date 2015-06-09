////////////////////////////////////////////////////////////////////////////////
// ADT_rovioAPI.h
// 	Mario Chirinos Colunga
// 	Oscar Sanchez Sioridia
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	17 Dic 2010 - 23 Nov 2011
//------------------------------------------------------------------------------
// Notes:
//	Rovio Wowwee API
//	www.myrovio.com
//
////////////////////////////////////////////////////////////////////////////////

#ifndef ADT_ROVIOAPI_H
#define ADT_ROVIOAPI_H

// your public header include
//------------------------------------------------------------------------------
#include "rovioStreaming.h"
#include "rovioSpeak.h"

#include <curl/curl.h>
#include <curl/easy.h>
//------------------------------------------------------------------------------
enum rovioValues
{
	NONEACTION,
	GETREPORT,
	STARTRECORDING,
	ABORTRECORDING,
	STOPRECORDING,
	DELETEPATH,
	GETPATHLIST,
	PLAYPATHFORWARD,
	PLAYPATHBACKWARDS,
	STOPPLAYING,
	PAUSEPLAYING,
	RENAMEPATH,
	GOHOME,
	GOHOMEANDDOCK,
	UPDATEHOMEPOSITION,
	SETTUNINGPARAMETERS,
	GETTUNINGPARAMETERS,
	RESETNAVSTATEMACHINE,
	MANUALDRIVE,
	RESERVED,
	GETMCUREPORT
};
enum d_value
{
	STOP,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	RCCW,
	RCW,
	DIAGFLEFT,
	DIAGFRIGHT,
	DIAGBLEFT,
	DIAGBRIGHT,
	HEADUP,
	HEADMIDDLE,
	HEADDOWN,
	RESERVED1,
	RESERVED2,
	RESERVED3,
	RCCW20,
	RCW20
};
//------------------------------------------------------------------------------
class ADT_rovioAPI
{
 private:
	string uri;
	int sendCommand(const char *command) const;
	static int curlCallback(char *data, unsigned int size, unsigned int nmemb, void *userdata);

 public:
 	rovioStreaming *streaming;
 	rovioSpeak *speak;
	void setHome() const;
	void goHome() const;
	void stop() const;
	void move(double angle, unsigned int speed) const;
	void rotate(double angle, unsigned int speed) const;
	void headUp() const;
	void headMiddle() const;
	void headDown() const;
	void light(int l) const;
	const char* getURI() const;
	ADT_rovioAPI(const char *_uri);
	~ADT_rovioAPI();
	
};
//------------------------------------------------------------------------------
#endif
