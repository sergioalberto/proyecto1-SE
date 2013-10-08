#include "reproductor.h"
#include <gst/gst.h>

GstElement *pipeline;
GstBus *bus;
guint bus_watch_id;
GMainLoop *loop;


reproductor::reproductor(){}

static void on_pad_added (GstElement *element, GstPad *pad, gpointer data) {

    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;

    // We can now link this pad with the vorbis-decoder sink pad
    g_print ("Dynamic pad created, linking demuxer/decoder\n");

    sinkpad = gst_element_get_static_pad (decoder, "sink");

    gst_pad_link (pad, sinkpad);

    gst_object_unref (sinkpad);
}



static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data) {

    GMainLoop *loop = (GMainLoop *) data;

    switch (GST_MESSAGE_TYPE (msg)) {

        case GST_MESSAGE_EOS:
            g_print ("End of stream\n");
            g_main_loop_quit (loop);
            break;

            case GST_MESSAGE_ERROR: {
                gchar  *debug;
                GError *error;

                gst_message_parse_error (msg, &error, &debug);
                g_free (debug);

                g_printerr ("Error: %s\n", error->message);
                g_error_free (error);

                g_main_loop_quit (loop);
                break;
            }
            default:
                break;
    }

    return TRUE;
}



//int reproduciendo;
void reproductor::play(char *name){

    GMainLoop *loop;
    GstBus *bus;
    GstElement *source;
    GstElement *filter;
    GstElement *sink;
    //GstElement *pipeline;
    //name="/home/luis/Escritorio/1.mp3";
    gst_init (NULL, NULL);
    loop     = g_main_loop_new (NULL, FALSE);
    pipeline = gst_pipeline_new ("mp3 player");
    source   = gst_element_factory_make ("filesrc", "file reader");
    filter   = gst_element_factory_make ("mad", "MP3 decoder");
    sink     = gst_element_factory_make ("alsasink", "ALSA output");

    g_object_set(G_OBJECT (source), "location", name, NULL);
    gst_bin_add_many (GST_BIN (pipeline), source, filter, sink, NULL);
    gst_element_link_many (source, filter, sink, NULL);
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);  // reproduciendo=1;
}

void reproductor::stop(){
    gst_element_set_state (pipeline, GST_STATE_NULL);
}

void reproductor::pause(){
    gst_element_set_state (pipeline, GST_STATE_PAUSED);
}


void reproductor::unpause(){
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
}


void reproductor::stream(char *name,char *ip, int port){
    //gst-launch filesrc location="1.mp3" ! mad ! audioconvert ! audioresample ! mulawenc ! \rtppcmupay ! udpsink host=172.18.84.135 auto-multicast=true port=9000
    GMainLoop *loop;
    GstBus *bus;
    GstElement *source;
    GstElement *filter;
    GstElement *convert;
    GstElement *rtppcmupay;
    GstElement *sink;
    GstElement *capsfilter ;
    //gst-launch filesrc location=1.mp3 ! mad ! audioconvert ! audio/x-raw-int,channels=1,
    //depth=16,width=16, rate=44100 ! rtpL16pay  ! udpsink host=224.0.0.15 port=5000;

    //name="/home/luis/Escritorio/1.mp3";
    //port = 5000;
    //ip ="224.0.0.15";
    //ip="192.168.11.125";
    gst_init (NULL, NULL);
    //gst_parse_launch("gst-launch filesrc location=1.mp3 ! mad ! audioconvert ! audio/x-raw-int,channels=1,depth=16,width=16, rate=44100 ! rtpL16pay  ! udpsink host=224.0.0.15 port=5000",NULL);

    loop     = g_main_loop_new (NULL, FALSE);
    pipeline = gst_pipeline_new ("mp3 stream");
    source   = gst_element_factory_make ("filesrc", "file reader");
    filter   = gst_element_factory_make ("mad", "MP3 decoder");
    convert  = gst_element_factory_make ("audioconvert", "audioconvert");
    capsfilter = gst_element_factory_make("capsfilter", "caps");

    GstCaps *caps = gst_caps_from_string("audio/x-raw-int,channels=1,depth=16,width=16, rate=44100");
    g_object_set (capsfilter, "caps", caps, NULL);
    gst_caps_unref (caps);

    rtppcmupay = gst_element_factory_make ("rtpL16pay", "rtpL16pay");
    sink     = gst_element_factory_make ("udpsink", "udpsink");

    g_object_set(G_OBJECT (source), "location",name,NULL);
    g_object_set(G_OBJECT (sink), "host",ip,"port",port, NULL);

    gst_bin_add_many (GST_BIN (pipeline), source, filter, convert,capsfilter,rtppcmupay,sink, NULL);
    gst_element_link_many (source, filter, convert,capsfilter,rtppcmupay,sink, NULL);
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);

}
