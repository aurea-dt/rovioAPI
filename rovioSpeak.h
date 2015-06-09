////////////////////////////////////////////////////////////////////////////////
// rovioSpeak.h
// 	Mario Chirinos Colunga
// 	Oscar Sanchez Sioridia
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	18 Dic 2010 - 26 Jun 2013
//------------------------------------------------------------------------------
// Notes:
//	send rovio audio using espeak and culr
//
////////////////////////////////////////////////////////////////////////////////

#ifndef ROVIOSPEAK_H
#define ROVIOSPEAK_H

// your public header include
//------------------------------------------------------------------------------
#include <iostream>
#include <espeak/speak_lib.h>

#include <curl/curl.h>
#include <curl/easy.h>

//------------------------------------------------------------------------------

// the declaration of your class...
using namespace std;
//------------------------------------------------------------------------------
class rovioSpeak
{
 private:
	string audiouri;
	int sampleRate;

	unsigned char* audioBuffer;
	int audioBufferLength;

	unsigned int rovioSR;
	void mergeAudioBuffers(unsigned char* buffer, unsigned int bufferLength);
	void sendBuffer(unsigned char* buffer, unsigned int bufferLength) const;
	void sendBuffer();
	int scaleBuffer(short int* wavIn, int nSamplesIn, short int* wavOut) const;
	static int SynthCallback(short int *wav, int numsamples, espeak_EVENT *events);

 public:
	void speak(const char* text);
	rovioSpeak(const char* _uri);
	~rovioSpeak();
};
//------------------------------------------------------------------------------
#endif
