#ifndef GESTIONVIDEO_H
#define GESTIONVIDEO_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <QList>

#if _WIN32
#define snprintf _snprintf
#endif
//#define __STDC_FORMAT_MACROS
//#define PRId64
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
extern "C" {
#include <libavutil\avassert.h>
#include <libavutil\channel_layout.h>
#include <libavutil\opt.h>
#include <libavutil\mathematics.h>
#include <libavutil\timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
#include <libavutil/mem.h>
#include <libavutil/pixdesc.h>
#include <libavutil/frame.h>
}

///Struct OutputStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;

///Struct FilteringContext
typedef struct FilteringContext {
    AVFilterContext *buffersink_ctx;
    AVFilterContext *buffersrc_ctx;
    AVFilterGraph *filter_graph;
} FilteringContext;

///Static FilteringContext
static FilteringContext *filter_ctx;


class GestionVideo
{
public:

    /**
     * @brief GestionVideo::GestionVideo Constructor
     * @param filename : name of video file
     */
    GestionVideo(std::string filename);

    /**
     * @brief Default destructor ; does nothing
     */
    ~GestionVideo();

    /**
     * @brief GestionVideo::extractVideo - Extracts the video
     * @param filenameOut: Name of out file
     * @param start: Time at start
     * @param end: Time at the end
     * @return the status of the process
     */
    int extractVideo(std::string filenameOut, int start, int end);


    /**
     * @brief GestionVideo::extractAudio - Extracts the sound
     * @param filenameOut: The name of the out file
     * @return the status of the process
     */
    int extractAudio(std::string filenameOut);


private:
    /*void readFrame(std::string filename);
    int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt);
    void add_stream(OutputStream *ost, AVFormatContext *oc,
                           AVCodec **codec,
                           enum AVCodecID codec_id);
    AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
                                      uint64_t channel_layout,
                                      int sample_rate, int nb_samples);
    void open_audio(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg);
    AVFrame *get_audio_frame(OutputStream *ost);
    int write_audio_frame(AVFormatContext *oc, OutputStream *ost);
    AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height);
    void open_video(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg);
    AVFrame *get_video_frame(OutputStream *ost);
    int write_video_frame(AVFormatContext *oc, OutputStream *ost);
    void close_stream(AVFormatContext *oc, OutputStream *ost);*/



    /**
     * @brief GestionVideo::open_input_file_extract_audio - Gets the sound extracted from the video and decode it
     * @param filename: the name of the video file
     * @return the status of the process
     */
    int open_input_file_extract_audio(const char *filename);


    /**
     * @brief GestionVideo::open_input_file_extract_video - Decode the video
     * @param filename: Name of the video file
     * @return The status of the process
     */
    int open_input_file_extract_video(const char *filename);

    /**
     * @brief GestionVideo::open_output_file_audio - Opens the sound file and encodes it
     * @param filename: The name of the file
     * @return AVCodecContext corresponding to this file
     */
    int open_output_file_audio(const char *filename);

    /**
     * @brief GestionVideo::open_output_file_video - Opens the video file and encodes it
     * @param filename: The name of the file
     * @return AVCodecContext corresponding to this file
     */
    int open_output_file_video(const char *filename);

    /**
     * @brief GestionVideo::init_filter - Initializes the filter
     * @param fctx: filter context
     * @param dec_ctx: codec context
     * @param enc_ctx: codec context
     * @param filter_spec
     * @return The status of the process
     */
    int init_filter(FilteringContext* fctx, AVCodecContext *dec_ctx,
                           AVCodecContext *enc_ctx, const char *filter_spec);

    /**
     * @brief GestionVideo::init_filters_video - - Initializes the video filter
     * @return The status of the process
     */
    int init_filters_video();


    /**
     * @brief GestionVideo::init_filters_audio - Initializes the sound filter
     * @return The status of the process
     */
    int init_filters_audio();

    /**
     * @brief GestionVideo::encode_write_frame - Encodes the filter
     * @param filt_frame: filter's frame
     * @param stream_index
     * @param got_frame
     * @return Status of the process
     */
    int encode_write_frame(AVFrame *filt_frame, unsigned int stream_index, int *got_frame);

    /**
     * @brief GestionVideo::filter_encode_write_frame - encodes AVFrame
     * @param frame: filter's frame
     * @param stream_index_in
     * @param stream_index_out
     * @return Status of the process
     */
    int filter_encode_write_frame(AVFrame *frame, unsigned int stream_index_in, unsigned int stream_index_out);

    /**
     * @brief GestionVideo::flush_encoder - Cleans the encoder
     * @param stream_index
     * @return Status of the process
     */
    int flush_encoder(unsigned int stream_index);

    /**
     * @brief GestionVideo::freeMemory - Frees the memories
     * @param packet: AVPacket
     * @param frame: AVFrame
     * @param filter_ctx: Filtering Context
     */
    void freeMemory(AVPacket * packet, AVFrame *frame, FilteringContext * filter_ctx);



protected:
    //Value that will store the duration of the video
    double duration;

    // Stores the number of frames, is used to get the time moving in the video
    double framerate;

    // Time of beginning of the video
    int start;

    // Time value at the end of the video
    int end;

    // Width of the window showing the video
    int width;

    // Height of the window showing the video
    int height;

    // Name of the file
    std::string filename;

    //
    AVFormatContext *ifmt_ctx;

    //
    AVFormatContext *ofmt_ctx;
};

#endif // GESTIONVIDEO_H
