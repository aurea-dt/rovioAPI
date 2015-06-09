////////////////////////////////////////////////////////////////////////////////
// 	rovioStreaming.cpp
// 	Mario Chirinos Colunga
// 	Oscar Sanchez Sioridia
// 	Áurea - Desarrollo Tecnológico
// 	http://www.aurea-dt.com
// 	17 Dic 2010 - 26 Jun 2013
//------------------------------------------------------------------------------
// Notes:
//	Media streaming using gstreamer
//
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string.h>
#include <sstream>
#include "rovioStreaming.h"
using namespace std;
//------------------------------------------------------------------------------
rovioStreaming::rovioStreaming(const char* _uri)
{
	stringstream ss;
	ss << "rtsp://" << _uri << "/webcam" << endl;
	getline(ss, videouri);
	userdata = NULL;
	video_window_xid = 0;

	//GTK+ video window
	videoWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	drawArea = gtk_drawing_area_new ();
	gtk_widget_set_double_buffered (drawArea, FALSE);
	gtk_container_add (GTK_CONTAINER (videoWindow), drawArea);
	video_window_xid = GDK_WINDOW_XID (drawArea->window);
//	if( g_signal_connect (drawArea, "realize", G_CALLBACK (video_widget_realize_cb), this) >=0)
//		cout << "signal -realize- connected to drawArea" << endl;
	g_signal_connect (videoWindow, "delete-event", G_CALLBACK (hideanddisconnect_cb), this);
	g_signal_connect (videoWindow, "hide", G_CALLBACK (hide_cb), this);
  	
	pipeline  = gst_pipeline_new("video-player");
	GstBus *bus;
	bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
	gst_bus_add_watch (bus, pipeline_bus_callback, NULL);
	gst_bus_set_sync_handler (bus, (GstBusSyncHandler) bus_sync_handler_cb, this);
	gst_object_unref(GST_OBJECT (bus));
	
	rtspSrc=NULL;
	videoDecoder=NULL;
	audioDecoder=NULL;
	colorSpaceConverter1=NULL;
	colorSpaceConverter2=NULL;
	videoSink=NULL;

	rtspSrc  = gst_element_factory_make("rtspsrc", "video_source_main");

	videoDecoder = gst_element_factory_make("decodebin2", "video_decoder");
	audioDecoder = gst_element_factory_make("decodebin2", "audio_decoder");
	videoFilter = gst_element_factory_make("identity", "video_filter");
 	videoFilterCaps    = gst_element_factory_make("capsfilter", "filter_cap");

	videoQueue = gst_element_factory_make("queue", "video_queue");
	colorSpaceConverter1  = gst_element_factory_make("ffmpegcolorspace", "colorSpaceConverter1");	
	colorSpaceConverter2  = gst_element_factory_make("ffmpegcolorspace", "colorSpaceConverter2");	
	videoSink = gst_element_factory_make("autovideosink", "video_sink");

	audioFilter = gst_element_factory_make("identity", "audio_filter");
	audioQueue = gst_element_factory_make("queue", "audio_queue");
	audioSink = gst_element_factory_make("autoaudiosink", "audio_sink");

	gst_bin_add_many (GST_BIN (pipeline), rtspSrc, videoDecoder, videoQueue, colorSpaceConverter1, videoFilter, videoFilterCaps, colorSpaceConverter2, videoSink, NULL);
	if(!gst_element_link_many(videoQueue, colorSpaceConverter1, videoFilterCaps, videoFilter, colorSpaceConverter2, videoSink, NULL))
	{
		cerr << "rovioStreaming(): Failed to link elements in the pipeline" << endl;
		return;
	}
	gst_bin_add_many (GST_BIN (pipeline), audioDecoder, audioQueue, audioFilter, audioSink, NULL);
	if(!gst_element_link_many(audioQueue, audioFilter, audioSink, NULL))
	{
		cerr << "rovioStreaming(): Failed to link elements in the pipeline" << endl;
		return;
	}

	g_signal_connect(rtspSrc, "pad-added", G_CALLBACK(srcNewPad_callback), this);
	g_signal_connect(videoDecoder, "pad-added", G_CALLBACK(videoDecoderNewPad_callback), this);
	g_signal_connect(audioDecoder, "pad-added", G_CALLBACK(audioDecoderNewPad_callback), this);
	gst_element_set_state (pipeline, GST_STATE_NULL);
}
//------------------------------------------------------------------------------
rovioStreaming::~rovioStreaming()
{

}
//------------------------------------------------------------------------------
void rovioStreaming::srcNewPad_callback(GstElement *element, GstPad *pad, void *data)
{
	cout << gst_element_get_name(element)<< " adding pad.." << gst_pad_get_name (pad) << endl;
	GstPad *sinkpad;
	string prefix = string(gst_pad_get_name (pad)).substr(0,14);

	if(strcmp(prefix.c_str(), "recv_rtp_src_0") == 0)
	{
		GstElement *nextElement = ((rovioStreaming*)data)->videoDecoder;
		sinkpad = gst_element_get_static_pad (nextElement, "sink");
		gst_pad_link (pad, sinkpad);
	}
	if(strcmp(prefix.c_str(), "recv_rtp_src_1") == 0)
	{
		GstElement *nextElement = ((rovioStreaming*)data)->audioDecoder;
		sinkpad = gst_element_get_static_pad (nextElement, "sink");
		gst_pad_link (pad, sinkpad);
	}
}
//------------------------------------------------------------------------------
void rovioStreaming::videoDecoderNewPad_callback(GstElement *element, GstPad *pad, void *data)
{
	cout << "videoDecoderNewPad: " << gst_element_get_name(element)<< " adding pad.." << gst_pad_get_name (pad) << endl;
	GstPad *sinkpad;
	GstElement *nextElement = ((rovioStreaming*)data)->videoQueue;
	sinkpad = gst_element_get_static_pad (nextElement, "sink");
	gst_pad_link (pad, sinkpad);
	gst_object_unref (sinkpad);
}
//------------------------------------------------------------------------------
void rovioStreaming::audioDecoderNewPad_callback(GstElement *element, GstPad *pad, void *data)
{
	cout << gst_element_get_name(element)<< " adding pad.." << gst_pad_get_name (pad) << endl;
	GstPad *sinkpad;
	GstElement *nextElement = ((rovioStreaming*)data)->audioQueue;
	sinkpad = gst_element_get_static_pad (nextElement, "sink");
	gst_pad_link (pad, sinkpad);
	gst_object_unref (sinkpad);

}
//------------------------------------------------------------------------------
int rovioStreaming::pipeline_bus_callback (GstBus *bus, GstMessage *message, void *data)
{
	printf("videoplayer Got %s message\n", GST_MESSAGE_TYPE_NAME (message));
	return 0;
}
//------------------------------------------------------------------------------
int rovioStreaming::connect()
{
	disconnect();
	cout << "video source: " << videouri << endl;
	g_object_set (G_OBJECT (rtspSrc), "location", videouri.c_str(), NULL);
	g_object_set (G_OBJECT (rtspSrc), "latency", 50, NULL);

	GstCaps *fc = gst_caps_new_full(gst_structure_new ("video/x-raw-rgb", NULL), NULL);
 	g_object_set(G_OBJECT (videoFilterCaps), "caps", fc, NULL);
	gst_element_set_state (pipeline, GST_STATE_PLAYING);

	GstPad *srcPad = gst_element_get_static_pad(videoFilter, "sink");
	GstCaps *videoCaps =gst_pad_get_negotiated_caps(srcPad);
	unsigned int nCaps = gst_caps_get_size(videoCaps);
	const GstStructure *str;
	cout << nCaps << " capabilities detected" << endl;
	for(unsigned int i=0; i < nCaps; i++)
	{
		int depth, fpsNumerator, fpsDenominator;	
		guint32 format;
		cout << " Capability: " << i << endl;
		str = gst_caps_get_structure (videoCaps, i);
		cout << gst_structure_get_name (str)<< ": fields = " << gst_structure_n_fields (str) << endl;

		if (!gst_structure_get_int (str, "width", &width) || !gst_structure_get_int (str, "height", &height))
			width = height = 0;
		if(!gst_structure_get_int (str, "bpp", &depth))
			depth = 0;
		if(!gst_structure_get_fourcc(str, "format", &format))
			format=GST_STR_FOURCC ("NONE");

		if(!gst_structure_get_fraction(str, "framerate", &fpsNumerator, &fpsDenominator))
		{
			fpsNumerator=0;
			fpsDenominator=0;
		}
		cout << ": "<< width << "x" << height<< " @ " << fpsNumerator <<"/" <<fpsDenominator <<" : "<< depth << " bits, " << endl;
	}

//	const GstStructure *str;
//	str = gst_caps_get_structure (videoCaps, 0);
//	if ((!gst_structure_get_int (str, "width", &width)) || !gst_structure_get_int (str, "height", &height))
//	{
//		cerr << "dimension information not found!" << endl;
//	}
	char *devsrc;
	g_object_get (G_OBJECT (rtspSrc), "location", &devsrc, NULL);
	gtk_window_set_title((GtkWindow *)videoWindow, devsrc);
	gtk_widget_show_all (videoWindow);
	return 0;
}
//------------------------------------------------------------------------------
int rovioStreaming::disconnect()
{
	gst_element_unlink (rtspSrc, videoFilter);
	gst_element_set_state (pipeline, GST_STATE_NULL);
	return 0;
}
//------------------------------------------------------------------------------
unsigned int rovioStreaming::getWidth() const
{
	return width;
}
//------------------------------------------------------------------------------
unsigned int rovioStreaming::getHeight() const
{
	return height;
}
//------------------------------------------------------------------------------
const char* rovioStreaming::getDev() const
{
	char * uri;
	g_object_get(G_OBJECT (rtspSrc), "location", &uri, NULL);
	return uri;
}
//------------------------------------------------------------------------------
void rovioStreaming::setVideoFilter(videoCallback fpt)
{
	videoFpt = fpt;
}
//------------------------------------------------------------------------------
void rovioStreaming::unsetVideoFilter()
{
	videoFpt = NULL;
}
//------------------------------------------------------------------------------
void rovioStreaming::setAudioFilter(rovioAudioCallback fpt)
{
	audioFpt = fpt;
}
//------------------------------------------------------------------------------
void rovioStreaming::unsetAudioFilter()
{
	audioFpt = NULL;
}
//------------------------------------------------------------------------------
int rovioStreaming::startVideoFilter() 
{
	g_object_set (G_OBJECT (videoFilter), "signal-handoffs", TRUE, NULL);
	return g_signal_connect (videoFilter, "handoff", G_CALLBACK (videoFilter_handoff_callback), this);
}
//------------------------------------------------------------------------------
int rovioStreaming::stopVideoFilter()
{
	return g_signal_handlers_disconnect_by_func(G_OBJECT (videoFilter), (void *)videoFilter_handoff_callback, this);
}
//------------------------------------------------------------------------------
int rovioStreaming::startAudioFilter()
{
	g_object_set (G_OBJECT (audioFilter), "signal-handoffs", TRUE, NULL);
	return g_signal_connect (audioFilter, "handoff", G_CALLBACK (audioFilter_handoff_callback), this);
}
//------------------------------------------------------------------------------
int rovioStreaming::stopAudioFilter()
{
	return g_signal_handlers_disconnect_by_func(G_OBJECT (audioFilter), (void *)audioFilter_handoff_callback, this);
}
//------------------------------------------------------------------------------
int rovioStreaming::videoFilter_handoff_callback(GstElement *filter, GstBuffer *buffer, void *user_data)
{
	gint _width =0;
	gint _height =0;
	GstPad *srcPad = gst_element_get_static_pad(filter, "sink");
	GstCaps *videoCaps =gst_pad_get_negotiated_caps(srcPad);
	const GstStructure *str;
	str = gst_caps_get_structure (videoCaps, 0);
	if ((!gst_structure_get_int (str, "width", &_width)) || !gst_structure_get_int (str, "height", &_height))
	{
		cerr << "dimension information not found!" << endl;
	}

	unsigned char *data = (unsigned char *)GST_BUFFER_DATA (buffer);

	if(((rovioStreaming*)user_data)->videoFpt!=NULL)// check if initialized
	{
		((rovioStreaming*)user_data)->videoFpt(data, _width, _height, 3, user_data); 
	}

return 0;
}
//------------------------------------------------------------------------------
int rovioStreaming::audioFilter_handoff_callback(GstElement *filter, GstBuffer *buffer, void *user_data)
{
	
	gint _width =0;
	gint _depth =0;
	gint _channels =0;
	GstPad *srcPad = gst_element_get_static_pad(filter, "sink");
////	((rovioStreaming*)user_data)->printCaps(srcPad);
	GstCaps *videoCaps =gst_pad_get_negotiated_caps(srcPad);
	const GstStructure *str;
	str = gst_caps_get_structure (videoCaps, 0);
	if ((!gst_structure_get_int (str, "width", &_width)) || !gst_structure_get_int (str, "depth", &_depth) || !gst_structure_get_int (str, "channels", &_channels))
	{
		cerr << "dimension information not found!" << endl;
	}
	unsigned char *data = (unsigned char *)GST_BUFFER_DATA (buffer);
	cout << "width: " << _width << " depth: " << _depth << " channels: " << _channels << endl;
	if(((rovioStreaming*)user_data)->audioFpt!=NULL)// check if initialized
	{
		//((rovioStreaming*)user_data)->videoFpt(data, _width, _height, 3, user_data); 
	}

return 0;
}
//------------------------------------------------------------------------------
void rovioStreaming::video_widget_realize_cb (GtkWidget* widget, void* user_data)
{
	#if GTK_CHECK_VERSION(2,18,0)
	  // This is here just for pedagogical purposes, GDK_WINDOW_XID will call
	  // it as well in newer Gtk versions
	  if (!gdk_window_ensure_native (widget->window))
	    g_error ("Couldn't create native window needed for GstXOverlay!");
	#endif

	#ifdef GDK_WINDOWING_X11
		cout << "GDK_WINDOWING_X11" << endl;
		((rovioStreaming*)user_data)->video_window_xid = GDK_WINDOW_XID (widget->window);
	#endif
}
//------------------------------------------------------------------------------
GstBusSyncReply rovioStreaming::bus_sync_handler_cb(GstBus * bus, GstMessage * message, void* user_data)
{
//	cout << "bus_sync_handler_cb" << endl;
	 // ignore anything but 'prepare-xwindow-id' element messages
	if (GST_MESSAGE_TYPE (message) != GST_MESSAGE_ELEMENT)
	{
		return GST_BUS_PASS;
	}
	if (!gst_structure_has_name (message->structure, "prepare-xwindow-id"))
	{
		return GST_BUS_PASS;
	}

 	((rovioStreaming*)user_data)->video_window_xid = GDK_WINDOW_XID (((rovioStreaming*)user_data)->drawArea->window);
	if (((rovioStreaming*)user_data)->video_window_xid != 0)
	{
		GstXOverlay *xoverlay;
//		// GST_MESSAGE_SRC (message) will be the video sink element
		xoverlay = GST_X_OVERLAY (GST_MESSAGE_SRC (message));
		gst_x_overlay_set_window_handle(xoverlay, ((rovioStreaming*)user_data)->video_window_xid);
 	}
	else
	{
		g_warning ("Should have obtained video_window_xid by now!");
	}
 
	gst_message_unref (message);
	return GST_BUS_DROP;
}
//------------------------------------------------------------------------------
void rovioStreaming::hide_cb(GtkObject *object, void* user_data)
{
	((rovioStreaming*)user_data)->disconnect();
}
//------------------------------------------------------------------------------
void rovioStreaming::hideanddisconnect_cb(GtkObject *object, void* user_data)
{
	gtk_widget_hide_on_delete((GtkWidget*)object);
}
//------------------------------------------------------------------------------
void rovioStreaming::printCaps(GstPad *pad)
{
	cout << " printCaps" << endl;
	guint32 format;
	GstCaps *videoCaps =gst_pad_get_negotiated_caps(pad);
	unsigned int nCaps = gst_caps_get_size(videoCaps);
	const GstStructure *str;
	cout << nCaps << " capabilities detected" << endl;
	for(unsigned int i=0; i < nCaps; i++)
	{
		cout << " Capability: " << i << endl;
		str = gst_caps_get_structure (videoCaps, i);
		cout << gst_structure_get_name (str)<< ": fields = " << gst_structure_n_fields (str) << endl;
		for(int f=0; f < gst_structure_n_fields (str); f++)
		{
			cout << gst_structure_nth_field_name(str, f) << endl;
		}
	}
}
//------------------------------------------------------------------------------
