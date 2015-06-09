////////////////////////////////////////////////////////////////////////////////
// rovioSpeak.cpp
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
#include "rovioSpeak.h"
#include <iostream>
#include <sstream>
#include <string.h>
using namespace std;
//------------------------------------------------------------------------------
rovioSpeak::rovioSpeak(const char *_uri)
{ 
	cout << "rovioSpeak("<< _uri <<")" << endl;
	sampleRate = espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 500, NULL, 1); 
	espeak_SetVoiceByName("default");
	espeak_SetSynthCallback(SynthCallback);

	stringstream ss;
	ss << "http://" << _uri << "/GetAudio.cgi" << endl;
	getline(ss, audiouri);

	rovioSR= 8000;
	audioBufferLength=0;
	audioBuffer=NULL;
	cout << audiouri << endl;
//	curl_global_init(CURL_GLOBAL_ALL);
}
//------------------------------------------------------------------------------
rovioSpeak::~rovioSpeak()
{
	espeak_Terminate();
}
//------------------------------------------------------------------------------
void rovioSpeak::speak(const char* text)
{
	cout << "speak(" << text << ")" << endl;

	string str = text;
//	unsigned int* unique_identifier;
	espeak_Synth(str.c_str(), str.size(), 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, this);
	espeak_Synchronize( );
}
//------------------------------------------------------------------------------
int rovioSpeak::SynthCallback(short int *wav, int numsamples, espeak_EVENT *events)
{
	cout << "int rovioSpeak::SynthCallback(short int *wav, "<< numsamples<< ", " << events->type << ")"<< endl;

	if(events->type==6)
	{
		cout << "End of message" << endl;
		((rovioSpeak*)events->user_data)->sendBuffer();
		return 0;
	}

	if(numsamples <= 0)
	{ cout << "no audio" << endl;
		return 0;
	}

	int newLenght = numsamples;
	short int *newBuffer = new short int[newLenght];
	newLenght=((rovioSpeak*)events->user_data)->scaleBuffer(wav, numsamples, newBuffer);
	string audiouri = ((rovioSpeak*)events->user_data)->audiouri;
	int bufferLength = newLenght*sizeof(short);

	((rovioSpeak*)events->user_data)->mergeAudioBuffers((unsigned char*)newBuffer, bufferLength);

	delete []newBuffer;
	return 0;
}
//------------------------------------------------------------------------------
void rovioSpeak::mergeAudioBuffers(unsigned char* buffer, unsigned int bufferLength)
{
	if(audioBuffer!=NULL)
	{
		unsigned char* audioTemp = new unsigned char [audioBufferLength];
		memcpy(audioTemp, audioBuffer,audioBufferLength);
		delete []audioBuffer;
		audioBuffer = new unsigned char [audioBufferLength+bufferLength];
		memcpy(audioBuffer, audioTemp, audioBufferLength);
		memcpy(audioBuffer+audioBufferLength, buffer, bufferLength);
		audioBufferLength+=bufferLength;
		delete []audioTemp;
	}
	else
	{
		cout << audioBufferLength << ", " << bufferLength << endl;
		audioBuffer = new unsigned char [audioBufferLength+bufferLength];
		memcpy(audioBuffer+audioBufferLength, buffer, bufferLength);
		audioBufferLength+=bufferLength;
	}

}
//------------------------------------------------------------------------------
void rovioSpeak::sendBuffer()
{
	sendBuffer(audioBuffer, audioBufferLength);
	delete []audioBuffer;
	audioBuffer = NULL;
	audioBufferLength = 0;
}
//------------------------------------------------------------------------------
void rovioSpeak::sendBuffer(unsigned char *buffer, unsigned int bufferLength) const
{
	CURL* ctx = curl_easy_init();
	cout << rovioSR << endl;
	cout << audiouri << endl;
	curl_easy_setopt(ctx, CURLOPT_URL, audiouri.c_str());
	curl_easy_setopt(ctx, CURLOPT_POST,true);	
	curl_easy_setopt(ctx, CURLOPT_POSTFIELDSIZE, bufferLength);
	curl_easy_setopt(ctx, CURLOPT_POSTFIELDS, buffer);
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
	curl_easy_setopt(ctx, CURLOPT_HTTPHEADER, headers);

	CURLcode ret = curl_easy_perform(ctx);
	curl_slist_free_all(headers);
	curl_easy_cleanup(ctx);
}
//------------------------------------------------------------------------------
int rovioSpeak::scaleBuffer(short int *wavIn, int nSamplesIn, short int *wavOut) const
{
	int newLenght = nSamplesIn*rovioSR/sampleRate;
	cout << nSamplesIn<<"*"<< rovioSR <<"/" << sampleRate << "=" << newLenght << endl;
	int delta = nSamplesIn/newLenght;
	for(int i=0; i<nSamplesIn; i+=delta)
	{
		*wavOut=wavIn[i];
		*wavOut++;
	}
	return newLenght;
}
//------------------------------------------------------------------------------

