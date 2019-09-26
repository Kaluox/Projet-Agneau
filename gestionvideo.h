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

typedef struct FilteringContext {
    AVFilterContext *buffersink_ctx;
    AVFilterContext *buffersrc_ctx;
    AVFilterGraph *filter_graph;
} FilteringContext;
static FilteringContext *filter_ctx;

class GestionVideo
{
public:
    GestionVideo(std::string filename);
    ~GestionVideo();

    int extractVideo(std::string filenameOut, int start, int end);
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



    int open_input_file_extract_audio(const char *filename);
    int open_input_file_extract_video(const char *filename);
    int open_output_file_audio(const char *filename);
    int open_output_file_video(const char *filename);
    int init_filter(FilteringContext* fctx, AVCodecContext *dec_ctx,
                           AVCodecContext *enc_ctx, const char *filter_spec);
    int init_filters_video();
    int init_filters_audio();
    int encode_write_frame(AVFrame *filt_frame, unsigned int stream_index, int *got_frame);
    int filter_encode_write_frame(AVFrame *frame, unsigned int stream_index_in, unsigned int stream_index_out);
    int flush_encoder(unsigned int stream_index);
    void freeMemory(AVPacket * packet, AVFrame *frame, FilteringContext * filter_ctx);

protected:
    int sws_flags;
    double duration;
    double framerate;
    int frameVideo;
    int frameAudio;
    int start;
    int end;
    int width;
    int height;
    int bitrate;
    bool haveVideo;
    bool haveAudio;
    std::string filename;
    AVPixelFormat stream_pix_fmt;
    QList<AVFrame *> videoFrames;
    QList<AVFrame *> audioFrames;
    AVFormatContext *ifmt_ctx;
    AVFormatContext *ofmt_ctx;
};

#endif // GESTIONVIDEO_H
