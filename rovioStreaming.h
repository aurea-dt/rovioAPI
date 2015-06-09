////////////////////////////////////////////////////////////////////////////////
// 	rovioStreaming.h
// 	Mario Chirinos Colunga
// 	Oscar Sanchez Sioridia
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	17 Dic 2010 - 22 Jun 2013
//------------------------------------------------------------------------------
// Notes:
//	Media streaming using gstreamer
//
////////////////////////////////////////////////////////////////////////////////

#ifndef IPCAM_H
#define IPCAM_H

// your public header include
#include <iostream>
#include <gst/gst.h>
#include <gtk/gtk.h>
#ifdef GDK_WINDOWING_X11
	#include <gdk/gdkx.h>  // for GDK_WINDOW_XID
#endif
#include <gst/interfaces/xoverlay.h>
//------------------------------------------------------------------------------
using namespace std;
typedef void (*videoCallback)(unsigned char*, unsigned int, unsigned int, unsigned int, void *);
typedef void (*rovioAudioCallback)(unsigned char*, unsigned int, unsigned int, unsigned int, void *);
//------------------------------------------------------------------------------
class rovioStreaming
{
 private:
 	string videouri;
	//video window
	unsigned long int video_window_xid;
	GtkWidget *videoWindow;
	GtkWidget *drawArea;

	// common elements
	GstElement *rtspSrc;
	GstElement *pipeline;

	//video elements
	int width;
	int height;

	GstElement *videoDecoder;
	GstElement *videoFilter;
	GstElement *videoFilterCaps;
	GstElement *videoQueue;	
	GstElement *colorSpaceConverter1;
	GstElement *colorSpaceConverter2;
        GstElement *videoSink; 
   
	// audio elements
	GstElement *audioDecoder;
	GstElement *audioFilter;
	GstElement *audioQueue;
	GstElement *audioSink;

	void static srcNewPad_callback(GstElement *element, GstPad *pad, void *data);
	void static videoDecoderNewPad_callback(GstElement *element, GstPad *pad, void *data);
	void static audioDecoderNewPad_callback(GstElement *element, GstPad *pad, void *data);

	static int pipeline_bus_callback(GstBus *bus, GstMessage *message, void *data);
	static int videoFilter_handoff_callback(GstElement *filter, GstBuffer *buffer, void *user_data);
	static int audioFilter_handoff_callback(GstElement *filter, GstBuffer *buffer, void *user_data);

	static void video_widget_realize_cb(GtkWidget* widget, void* user_data);
	static GstBusSyncReply bus_sync_handler_cb(GstBus * bus, GstMessage * message, void* user_data);
	static void hideanddisconnect_cb(GtkObject *object, void* user_data);
	static void hide_cb(GtkObject *object, void* user_data);

	videoCallback videoFpt;
	rovioAudioCallback audioFpt;
 public:
	string name;
	void* userdata;

	int connect();
	int disconnect();
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	const char* getDev() const;
	void setVideoFilter(videoCallback ftp);
	void unsetVideoFilter();
	void setAudioFilter(rovioAudioCallback ftp);
	void unsetAudioFilter();
	int startAudioFilter();
	int stopAudioFilter();
	int startVideoFilter();
	int stopVideoFilter();

void printCaps(GstPad *pad);

	rovioStreaming(const char* _uri);
	~rovioStreaming();
};
//------------------------------------------------------------------------------
#endif
