#include "streamer.h"
#include <gst/gst.h>

streamer::streamer()
{
}

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


GstElement *pipeline;

void streamer::stream( int port){

    //gst-launch-0.10 udpsrc port=5000 ! "application/x-rtp,media=(string)audio, clock-rate=(int)44100, width=16,
    //height=16, encoding-name=(string)L16, encoding-params=(string)1, channels=(int)1, channel-positions=(int)1,
    //payload=(int)96" ! gstrtpjitterbuffer do-lost=true ! rtpL16depay ! audioconvert ! alsasink sync=false


    GMainLoop *loop;
    GstBus *bus;
    GstElement *source;
    GstElement *filter;
    GstElement *convert;
    GstElement *rtppcmupay;
    GstElement *sink;
    GstElement *capsfilter ;


    //name="/home/luis/Escritorio/1.mp3";
    port = 5000;
    //ip ="224.0.0.15";
    gst_init (NULL, NULL);
    //gst_parse_launch("gst-launch filesrc location=1.mp3 ! mad ! audioconvert ! audio/x-raw-int,channels=1,depth=16,width=16, rate=44100 ! rtpL16pay  ! udpsink host=224.0.0.15 port=5000",NULL);

    loop     = g_main_loop_new (NULL, FALSE);
    pipeline = gst_pipeline_new ("mp3 stream reciever");
    source   = gst_element_factory_make ("udpsrc", "file reader");
    capsfilter = gst_element_factory_make("capsfilter", "caps");
    filter   = gst_element_factory_make ("gstrtpjitterbuffer", "buffer");
    rtppcmupay = gst_element_factory_make ("rtpL16depay", "rtpL16pay");
     convert  = gst_element_factory_make ("audioconvert", "audioconvert");
    sink     = gst_element_factory_make ("alsasink", "alsasink");

    GstCaps *caps = gst_caps_from_string("application/x-rtp,media=(string)audio, clock-rate=(int)44100, width=16,height=16, encoding-name=(string)L16, encoding-params=(string)1, channels=(int)1, channel-positions=(int)1,payload=(int)96");
    g_object_set (capsfilter, "caps", caps, NULL);
    gst_caps_unref (caps);



    g_object_set(G_OBJECT (source), "port",port,NULL);
    g_object_set(G_OBJECT (sink), "sync",false, NULL);

    gst_bin_add_many (GST_BIN (pipeline), source, capsfilter,filter,rtppcmupay,convert,sink, NULL);
    gst_element_link_many (source, capsfilter,filter,rtppcmupay,convert,sink, NULL);
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);

}

void streamer::stop(){

 gst_element_set_state (pipeline, GST_STATE_NULL);

}



