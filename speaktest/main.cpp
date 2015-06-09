////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <espeak/speak_lib.h>
#include <curl/curl.h>
#include <curl/easy.h>
using namespace std;
class rovioSpeak
{
 private:
	int sampleRate;
	string audiouri;
	int rovioSR;
	unsigned char* audioBuffer;
	int audioBufferLength;

	static int SynthCallback(short int *wav, int numsamples, espeak_EVENT *events);
	void mergeAudioBuffers(unsigned char* buffer, unsigned int bufferLength);
	void sendBuffer();
	void sendBuffer(unsigned char *buffer, unsigned int bufferLength) const;
	int scaleBuffer(short int *wavIn, int nSamplesIn, short int *wavOut) const;
 public:
	rovioSpeak(const char* uri);
	void speak(const char* text);
};
//------------------------------------------------------------------------------
rovioSpeak::rovioSpeak(const char* uri)
{
	sampleRate = espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 500, NULL, 1);
	cout << "espeak sample rate: " << sampleRate <<endl;
	espeak_SetVoiceByName("default");
	espeak_SetSynthCallback(SynthCallback);
	rovioSR= 8000;
	audioBufferLength=0;
	audioBuffer=NULL;

	stringstream ss;
	ss << "http://" << uri << "/GetAudio.cgi" << endl;
	getline(ss, audiouri);
}
//------------------------------------------------------------------------------
int rovioSpeak::SynthCallback(short int *wav, int numsamples, espeak_EVENT *events)
{
	cout << "int rovioSpeak::SynthCallback(short int *wav, int numsamples, espeak_EVENT *events)"<< endl;
	cout << "Samples: " << numsamples << endl;
	cout << "Event type: " << events->type << endl;

	if(events->type==6)
	{
		cout << "End of message" << endl;
		((rovioSpeak*)events->user_data)->sendBuffer();
		return 1;
	}

	if(numsamples <= 0)
	{ cout << "no audio" << endl;
		return 1;
	}

	short int *newBuffer = new short int[numsamples];
	int newLenght=((rovioSpeak*)events->user_data)->scaleBuffer(wav, numsamples, newBuffer);
	cout << endl << "AUDIO " << ((rovioSpeak*)events->user_data)->audiouri << endl;
	int bufferLength = newLenght*sizeof(short);

	((rovioSpeak*)events->user_data)->mergeAudioBuffers((unsigned char*)newBuffer, bufferLength);

	delete []newBuffer;

	return 0;
}
//------------------------------------------------------------------------------
void rovioSpeak::speak(const char* text)
{
	string str = text;
	unsigned int* unique_identifier;
	espeak_Synth(str.c_str(), str.size(), 0, POS_CHARACTER, 0, espeakCHARS_AUTO, unique_identifier, this);
	espeak_Synchronize( );
}
//------------------------------------------------------------------------------
int rovioSpeak::scaleBuffer(short int *wavIn, int nSamplesIn, short int *wavOut) const
{
	cout << "scaleBuffer" << sampleRate << endl;
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
void rovioSpeak::mergeAudioBuffers(unsigned char* buffer, unsigned int bufferLength)
{
	if(audioBuffer!=NULL)
	{
		cout << "audioBuffer_!=NULL"<< endl;
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
		cout << "audioBuffer_==NULL"<< endl;
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
int mySynthCallback(short int *wav, int numsamples, espeak_EVENT *events)
{
	cout << "int rovioSpeak::SynthCallback(short int *wav, int numsamples, espeak_EVENT *events)"<< endl;
	cout << "Samples: " << numsamples << endl;
	cout << "Event type: " << events->type << endl;



	return 0;
}
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	if(argc < 0)
	{
		cerr << "too few arguments" << endl;
		return EXIT_FAILURE;
	}
	rovioSpeak* mySpeak = new rovioSpeak("192.168.1.65");
	mySpeak->speak("Rovio online");
	int i; 
	cin >> i;
	cout << i << endl;
	mySpeak->speak("Rovio online");

 return 0;
}
//------------------------------------------------------------------------------
