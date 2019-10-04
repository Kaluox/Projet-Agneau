#include "gestionvideo.h"
#include <QProcess>
#include <QDebug>

/**
 * @brief GestionVideo::GestionVideo constructeur
 * @param filename : le nom de fichier video
 */
GestionVideo::GestionVideo(std::string filename)
{
    this->filename = filename;
    ofmt_ctx = NULL;
    ifmt_ctx = NULL;
    av_register_all();
}

GestionVideo::~GestionVideo()
{

}

/**
 * @brief GestionVideo::extractVideo - extraire (couper) la video
 * @param filenameOut: le nom de fichier de retour
 * @param start: le temps commence
 * @param end: le temps finit
 * @return le statut du processus
 */
int GestionVideo::extractVideo(std::string filenameOut, int start, int end)
{
    this->start = start;
    this->end = end;
    int ret;
    AVPacket packet;
    packet.data = nullptr;
    packet.size = 0;
    AVFrame *frame = nullptr;
    enum AVMediaType type;
    unsigned int stream_index;
    //unsigned int i;
    int got_frame;
    int (*dec_func)(AVCodecContext *, AVFrame *, int *, const AVPacket *);

    //av_register_all();
    //avfilter_register_all();
    /*int start = 0;
    int end = 10;*/
    if ((ret = open_input_file_extract_video(filename.c_str())) < 0)
    {
        freeMemory(&packet, frame, filter_ctx);
        return 0;
    }
    if ((ret = open_output_file_video(filenameOut.c_str())) < 0)
    {
        freeMemory(&packet, frame, filter_ctx);
        return 0;
    }
    if ((ret = init_filters_video()) < 0)
    {
        freeMemory(&packet, frame, filter_ctx);
        return 0;
    }
    /* read all packets */
    int nbFrameVideo = 0;
    int nbFrameAudio = 0;
    bool stop = false;
    AVFrame * pFrame;
    int got = 0;
    double duration;
    double framerateAudio = 0;
    double framerateVideo = 0;
    for(int i = 0; i < ifmt_ctx->nb_streams; i++){
        AVStream * stream = ifmt_ctx->streams[i];
        if(stream->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            framerateVideo = stream->avg_frame_rate.num / (double)(stream->avg_frame_rate.den);
        }
        else if(stream->codec->codec_type == AVMEDIA_TYPE_AUDIO){
            duration = stream->duration * stream->time_base.num / (double)(stream->time_base.den);
            framerateAudio = stream->nb_frames / (double)(duration);
        }
    }
    int frameVideoStart = start * framerateVideo;
    int frameVideoEnd = (end * framerateVideo) + 1;
    int frameAudioStart = 0;
    int frameAudioEnd = 0;
    if(framerateAudio != 0){
        frameAudioStart = start * framerateAudio;
        frameAudioEnd = (end * framerateAudio) + 1;
    }

    while(!stop && start != 0){
        pFrame = av_frame_alloc();
        if ((ret = av_read_frame(ifmt_ctx, &packet)) < 0)
            stop = true;
        if(!stop && packet.stream_index == 0 && nbFrameVideo != frameVideoStart){
            avcodec_decode_video2(ifmt_ctx->streams[packet.stream_index]->codec, pFrame, &got, &packet);
            nbFrameVideo++;
        }
        else if (!stop && nbFrameAudio != frameAudioStart){
            avcodec_decode_audio4(ifmt_ctx->streams[packet.stream_index]->codec, pFrame, &got, &packet);
            nbFrameAudio++;
        }
        if(nbFrameVideo >= frameVideoStart && nbFrameAudio >= frameAudioStart)
            stop = true;
        av_frame_unref(pFrame);
        av_packet_unref(&packet);
    }
    int ptsVideo = 0;
    int ptsAudio = 0;
    stop = false;
    while (!stop) {
        if ((ret = av_read_frame(ifmt_ctx, &packet)) < 0)
            stop = true;
        else{
            stream_index = packet.stream_index;
            type = ifmt_ctx->streams[packet.stream_index]->codec->codec_type;
            //av_log(NULL, AV_LOG_DEBUG, "Demuxer gave frame of stream_index %u\n", stream_index);
            if (filter_ctx[stream_index].filter_graph) {
                //av_log(NULL, AV_LOG_DEBUG, "Going to reencode&filter the frame\n");
                frame = av_frame_alloc();
                if (!frame) {
                    ret = AVERROR(ENOMEM);
                }
                else {
                    av_packet_rescale_ts(&packet,
                                         ifmt_ctx->streams[stream_index]->time_base,
                                         ifmt_ctx->streams[stream_index]->codec->time_base);
                    if(type == AVMEDIA_TYPE_VIDEO){
                        dec_func = avcodec_decode_video2;
                    }
                    else{
                        dec_func = avcodec_decode_audio4;
                    }
                    /*dec_func = (type == AVMEDIA_TYPE_VIDEO) ? avcodec_decode_video2 :
                        avcodec_decode_audio4;*/
                    ret = dec_func(ifmt_ctx->streams[stream_index]->codec, frame,
                            &got_frame, &packet);
                    if (ret < 0) {
                        av_frame_free(&frame);
                        av_log(NULL, AV_LOG_ERROR, "Decoding failed\n");
                    }
                    else{
                        if (got_frame) {
                            if(packet.stream_index == 0 && nbFrameVideo != frameVideoEnd){
                                frame->pts = ptsVideo;
                                ptsVideo += packet.duration;//(1/(double)framerateVideo) / (ofmt_ctx->streams[packet.stream_index]->time_base.num / (double)(ofmt_ctx->streams[packet.stream_index]->time_base.den));
                                nbFrameVideo++;
                                ret = filter_encode_write_frame(frame, stream_index, stream_index);
                                av_frame_free(&frame);
                                if (ret < 0){
                                    freeMemory(&packet, frame, filter_ctx);
                                    return 0;
                                }
                            }
                            else if(packet.stream_index == 1 && nbFrameAudio != frameAudioEnd){
                                frame->pts = ptsAudio;
                                ptsAudio += packet.duration;//(1/(double)framerateAudio) / (ofmt_ctx->streams[packet.stream_index]->time_base.num / (double)(ofmt_ctx->streams[packet.stream_index]->time_base.den));
                                nbFrameAudio++;
                                ret = filter_encode_write_frame(frame, stream_index, stream_index);
                                av_frame_free(&frame);
                                if (ret < 0){
                                    freeMemory(&packet, frame, filter_ctx);
                                    return 0;
                                }
                            }
                            else{
                                av_frame_free(&frame);
                            }

                        } else {
                            av_frame_free(&frame);
                        }
                    }


                }

            } else {
                /* remux this frame without reencoding */
                av_packet_rescale_ts(&packet,
                                     ifmt_ctx->streams[stream_index]->time_base,
                                     ofmt_ctx->streams[stream_index]->time_base);
                ret = av_interleaved_write_frame(ofmt_ctx, &packet);
                if (ret < 0){
                    freeMemory(&packet, frame, filter_ctx);
                    return 0;
                }
            }
            av_packet_unref(&packet);
            if(nbFrameVideo >= frameVideoEnd && nbFrameAudio >= frameAudioEnd){
                stop = true;
            }
        }

    }
    /* flush filters and encoders */
    for (unsigned int i = 0; i < ifmt_ctx->nb_streams; i++) {
        /* flush filter */
        if (!filter_ctx[i].filter_graph)
            continue;
        ret = filter_encode_write_frame(NULL, i, i);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Flushing filter failed\n");
            freeMemory(&packet, frame, filter_ctx);
            return 0;
        }
        /* flush encoder */
        ret = flush_encoder(i);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Flushing encoder failed\n");
            freeMemory(&packet, frame, filter_ctx);
            return 0;
        }
    }
    av_write_trailer(ofmt_ctx);
    freeMemory(&packet, frame, filter_ctx);
    return 0;
}

/**
 * @brief GestionVideo::extractAudio - extraire le son
 * @param filenameOut: le nom de fichier retour
 * @return le statut de processus
 */
int GestionVideo::extractAudio(std::string filenameOut)
{    
    qDebug()<< "Extract Audio";
    QProcess extractingProcess;
    extractingProcess.start("ffmpeg", QStringList() << "-i" << QString::fromStdString(filename) << QString::fromStdString(filenameOut));

    if (!extractingProcess.waitForFinished())
    {
        return 1;
    }

    return 0;
}

/**
 * @brief GestionVideo::open_input_file_extract_audio - récupérer le son vers le vidéo et decoder le son
 * @param filename: le nom de fichier vidéo
 * @return le statut de processus
 */
int GestionVideo::open_input_file_extract_audio(const char *filename)
{
    int ret;
    unsigned int i;

    ifmt_ctx = NULL;
    if ((ret = avformat_open_input(&ifmt_ctx, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }
    bool findAudio = false;
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *stream;
        AVCodecContext *codec_ctx;
        stream = ifmt_ctx->streams[i];
        codec_ctx = stream->codec;
        /* Reencode video & audio and remux subtitles etc. */
        if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
                || codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            if(codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO){
                findAudio = true;
            }
            /* Open decoder */
            ret = avcodec_open2(codec_ctx,
                                avcodec_find_decoder(codec_ctx->codec_id), NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
                return ret;
            }
        }
    }
    if(!findAudio)
        return - 1;
    av_dump_format(ifmt_ctx, 0, filename, 0);
    return 0;
}

/**
 * @brief GestionVideo::open_input_file_extract_video - décoder la vidéo
 * @param filename: le nom de fichier vidéo
 * @return le statut de processus
 */
int GestionVideo::open_input_file_extract_video(const char *filename)
{
    int ret;
    unsigned int i;
    ifmt_ctx = NULL;
    if ((ret = avformat_open_input(&ifmt_ctx, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }
    if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *stream;
        AVCodecContext *codec_ctx;
        stream = ifmt_ctx->streams[i];
        codec_ctx = stream->codec;
        /* Reencode video & audio and remux subtitles etc. */
        if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
                || codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* Open decoder */
            ret = avcodec_open2(codec_ctx,
                    avcodec_find_decoder(codec_ctx->codec_id), NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
                return ret;
            }
        }
    }
    av_dump_format(ifmt_ctx, 0, filename, 0);
    return 0;
}

/**
 * @brief GestionVideo::open_output_file_audio - ouvrir le fichier de son et encoder le fichier
 * @param filename: le nom de fichier
 * @return AVCodecContext correspond à ce fichier
 */
int GestionVideo::open_output_file_audio(const char *filename)
{
    AVStream *out_stream;
    AVStream *in_stream;
    AVCodecContext *dec_ctx, *enc_ctx;
    AVCodec *encoder;
    int ret;
    unsigned int i;

    ofmt_ctx = NULL;
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, filename);
    if (!ofmt_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return AVERROR_UNKNOWN;
    }


    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        if(ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            out_stream = avformat_new_stream(ofmt_ctx, NULL);
            if (!out_stream) {
                av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
                return AVERROR_UNKNOWN;
            }

            in_stream = ifmt_ctx->streams[i];
            dec_ctx = in_stream->codec;
            enc_ctx = out_stream->codec;

            // in this example, we choose transcoding to same codec
            encoder = avcodec_find_encoder(AV_CODEC_ID_PCM_S16LE);
            if (!encoder) {
                av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
                return AVERROR_INVALIDDATA;
            }
            enc_ctx->sample_rate = 44100;//dec_ctx->sample_rate;
            enc_ctx->channel_layout =AV_CH_LAYOUT_STEREO;// dec_ctx->channel_layout;
            enc_ctx->channels = 2;//av_get_channel_layout_nb_channels(enc_ctx->channel_layout);
            // take first format from list of supported formats
            enc_ctx->sample_fmt = AV_SAMPLE_FMT_S16;//encoder->sample_fmts[0];
            enc_ctx->time_base = {1, enc_ctx->sample_rate};
            //}

            // Third parameter can be used to pass settings to encoder
            ret = avcodec_open2(enc_ctx, encoder, NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", i);
                return ret;
            }
            /*if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
                enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;*/
        }



    }
    av_dump_format(ofmt_ctx, 0, filename, 1);

    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", filename);
            return ret;
        }
    }

    /* init muxer, write output file header */
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        //av_log(NULL, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return ret;
    }

    return 0;
}

/**
 * @brief GestionVideo::open_output_file_video - ouvrir la vidéo et encoder
 * @param filename: le nom de fichier
 * @return AVCodecContext correspond à ce fichier
 */
int GestionVideo::open_output_file_video(const char *filename)
{
    AVStream *out_stream;
    AVStream *in_stream;
    AVCodecContext *dec_ctx, *enc_ctx;
    AVCodec *encoder;
    int ret;
    unsigned int i;
    ofmt_ctx = NULL;
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, filename);
    if (!ofmt_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return AVERROR_UNKNOWN;
    }
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        if(ifmt_ctx->streams[i]->codec->codec_id == AV_CODEC_ID_MPEG4)
            encoder = avcodec_find_encoder(AV_CODEC_ID_H264);
        else if(ifmt_ctx->streams[i]->codec->codec_id == AV_CODEC_ID_MJPEG)
        {
            encoder = avcodec_find_encoder(AV_CODEC_ID_H264);
        }
        else{
            encoder = avcodec_find_encoder(ifmt_ctx->streams[i]->codec->codec_id);
        }
        out_stream = avformat_new_stream(ofmt_ctx, encoder);
        if (!out_stream) {
            av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
            return AVERROR_UNKNOWN;
        }
        in_stream = ifmt_ctx->streams[i];
        dec_ctx = in_stream->codec;
        enc_ctx = out_stream->codec;
        if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
                || dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* in this example, we choose transcoding to same codec */

            if (!encoder) {
                av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
                return AVERROR_INVALIDDATA;
            }
            /* In this example, we transcode to same properties (picture size,
             * sample rate etc.). These properties can be changed for output
             * streams easily using filters */
            if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
                enc_ctx->height = dec_ctx->height;
                enc_ctx->width = dec_ctx->width;
                enc_ctx->sample_aspect_ratio = dec_ctx->sample_aspect_ratio;
                /* take first format from list of supported formats */
                if (encoder->pix_fmts)
                    enc_ctx->pix_fmt = encoder->pix_fmts[0];
                else
                    enc_ctx->pix_fmt = dec_ctx->pix_fmt;
                /* video time_base can be set to whatever is handy and supported by encoder */
                enc_ctx->time_base = dec_ctx->time_base;
            } else {
                enc_ctx->sample_rate = dec_ctx->sample_rate;
                enc_ctx->channel_layout = dec_ctx->channel_layout;
                enc_ctx->channels = av_get_channel_layout_nb_channels(enc_ctx->channel_layout);
                /* take first format from list of supported formats */
                enc_ctx->sample_fmt = encoder->sample_fmts[0];
                enc_ctx->time_base = {1, enc_ctx->sample_rate};
            }
            /* Third parameter can be used to pass settings to encoder */
            ret = avcodec_open2(enc_ctx, encoder, NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", i);
                return ret;
            }
        } else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            av_log(NULL, AV_LOG_FATAL, "Elementary stream #%d is of unknown type, cannot proceed\n", i);
            return AVERROR_INVALIDDATA;
        } else {
            /* if this stream must be remuxed */
            ret = avcodec_copy_context(ofmt_ctx->streams[i]->codec,
                    ifmt_ctx->streams[i]->codec);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Copying stream context failed\n");
                return ret;
            }
        }
        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
            enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    av_dump_format(ofmt_ctx, 0, filename, 1);
    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", filename);
            return ret;
        }
    }
    /* init muxer, write output file header */
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return ret;
    }
    return 0;
}

/**
 * @brief GestionVideo::init_filter - initialiser le filter
 * @param fctx: filter context
 * @param dec_ctx: codec context
 * @param enc_ctx: codec context
 * @param filter_spec
 * @return le statut de processus
 */
int GestionVideo::init_filter(FilteringContext* fctx, AVCodecContext *dec_ctx,
                       AVCodecContext *enc_ctx, const char *filter_spec)
{
    int ret = 0;
    const AVFilter *buffersrc = NULL;
    const AVFilter *buffersink = NULL;
    AVFilterContext *buffersrc_ctx = NULL;
    AVFilterContext *buffersink_ctx = NULL;
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs  = avfilter_inout_alloc();
    AVFilterGraph *filter_graph = avfilter_graph_alloc();

    if (!outputs || !inputs || !filter_graph) {
        ret = AVERROR(ENOMEM);
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        return ret;
    }

    if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
        buffersrc = avfilter_get_by_name("buffer");
        buffersink = avfilter_get_by_name("buffersink");
        if (!buffersrc || !buffersink) {
            av_log(NULL, AV_LOG_ERROR, "filtering source or sink element not found\n");
            ret = AVERROR_UNKNOWN;
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }
        QString qArgs;
        qArgs.sprintf("video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
                      dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
                      dec_ctx->time_base.num, dec_ctx->time_base.den,
                      dec_ctx->sample_aspect_ratio.num,
                      dec_ctx->sample_aspect_ratio.den);

        ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                                           qArgs.toStdString().c_str(), NULL, filter_graph);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot create buffer source\n");
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }

        ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                           NULL, NULL, filter_graph);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot create buffer sink\n");
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }

        ret = av_opt_set_bin(buffersink_ctx, "pix_fmts",
                             (uint8_t*)&enc_ctx->pix_fmt, sizeof(enc_ctx->pix_fmt),
                             AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot set output pixel format\n");
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }
    } else if (dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
        buffersrc = avfilter_get_by_name("abuffer");
        buffersink = avfilter_get_by_name("abuffersink");
        if (!buffersrc || !buffersink) {
            av_log(NULL, AV_LOG_ERROR, "filtering source or sink element not found\n");
            ret = AVERROR_UNKNOWN;
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }

        if (!dec_ctx->channel_layout)
            dec_ctx->channel_layout =
                    av_get_default_channel_layout(dec_ctx->channels);
        QString qArgs;
        qArgs.sprintf("time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%" PRIx64, dec_ctx->time_base.num,
                      dec_ctx->time_base.den,
                      dec_ctx->sample_rate,
                      av_get_sample_fmt_name(dec_ctx->sample_fmt),
                      dec_ctx->channel_layout);
        ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                                           qArgs.toStdString().c_str(), NULL, filter_graph);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer source\n");
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }

        ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                           NULL, NULL, filter_graph);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer sink\n");
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }

        ret = av_opt_set_bin(buffersink_ctx, "sample_fmts",
                             (uint8_t*)&enc_ctx->sample_fmt, sizeof(enc_ctx->sample_fmt),
                             AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot set output sample format\n");
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }

        ret = av_opt_set_bin(buffersink_ctx, "channel_layouts",
                             (uint8_t*)&enc_ctx->channel_layout,
                             sizeof(enc_ctx->channel_layout), AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot set output channel layout\n");
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }

        ret = av_opt_set_bin(buffersink_ctx, "sample_rates",
                             (uint8_t*)&enc_ctx->sample_rate, sizeof(enc_ctx->sample_rate),
                             AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot set output sample rate\n");
            avfilter_inout_free(&inputs);
            avfilter_inout_free(&outputs);

            return ret;
        }
    } else {
        ret = AVERROR_UNKNOWN;
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        return ret;
    }

    /* Endpoints for the filter graph. */
    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx    = 0;
    outputs->next       = NULL;

    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;

    if (!outputs->name || !inputs->name) {
        ret = AVERROR(ENOMEM);
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        return ret;
    }

    if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_spec,
                                        &inputs, &outputs, NULL)) < 0){
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        return ret;
    }
    if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0){
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        return ret;
    }

    /* Fill FilteringContext */
    fctx->buffersrc_ctx = buffersrc_ctx;
    fctx->buffersink_ctx = buffersink_ctx;
    fctx->filter_graph = filter_graph;
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);

    return ret;
}

/**
 * @brief GestionVideo::init_filters_audio - initialiser filter de son
 * @return le statut de processus
 */
int GestionVideo::init_filters_audio()
{
    const char *filter_spec;
    int ret;
    filter_ctx = (FilteringContext *)av_malloc_array(1, sizeof(*filter_ctx));
    if (!filter_ctx)
        return AVERROR(ENOMEM);
    filter_ctx[0].buffersrc_ctx  = NULL;
    filter_ctx[0].buffersink_ctx = NULL;
    filter_ctx[0].filter_graph   = NULL;
    filter_spec = "anull";
    ret = init_filter(&filter_ctx[0], ifmt_ctx->streams[1]->codec,
            ofmt_ctx->streams[0]->codec, filter_spec);
    if (ret)
        return ret;
    return 0;
}

/**
 * @brief GestionVideo::init_filters_video - - initialiser filter de vidéo
 * @return le statut de processus
 */
int GestionVideo::init_filters_video()
{
    const char *filter_spec;
    unsigned int i;
    int ret;
    filter_ctx = (FilteringContext *)av_malloc_array(ifmt_ctx->nb_streams, sizeof(*filter_ctx));
    if (!filter_ctx)
        return AVERROR(ENOMEM);
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        filter_ctx[i].buffersrc_ctx  = NULL;
        filter_ctx[i].buffersink_ctx = NULL;
        filter_ctx[i].filter_graph   = NULL;
        if (!(ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO
                || ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO))
            continue;
        if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            filter_spec = "null"; /* passthrough (dummy) filter for video */
        else
            filter_spec = "anull"; /* passthrough (dummy) filter for audio */
        ret = init_filter(&filter_ctx[i], ifmt_ctx->streams[i]->codec,
                ofmt_ctx->streams[i]->codec, filter_spec);
        if (ret)
            return ret;
    }
    return 0;
}

/**
 * @brief GestionVideo::encode_write_frame - encoder le filter
 * @param filt_frame: frame de filter
 * @param stream_index
 * @param got_frame
 * @return le statut de processus
 */
int GestionVideo::encode_write_frame(AVFrame *filt_frame, unsigned int stream_index, int *got_frame)
{
    int ret;
    int got_frame_local;
    AVPacket enc_pkt;
    int (*enc_func)(AVCodecContext *, AVPacket *, const AVFrame *, int *) =
            (ofmt_ctx->streams[stream_index]->codec->codec_type ==
            AVMEDIA_TYPE_VIDEO) ? avcodec_encode_video2 : avcodec_encode_audio2;

    if (!got_frame)
        got_frame = &got_frame_local;

    //av_log(NULL, AV_LOG_INFO, "Encoding frame\n");
    /* encode filtered frame */
    enc_pkt.data = NULL;
    enc_pkt.size = 0;
    av_init_packet(&enc_pkt);
    ret = enc_func(ofmt_ctx->streams[stream_index]->codec, &enc_pkt,
                   filt_frame, got_frame);
    av_frame_free(&filt_frame);
    if (ret < 0)
        return ret;
    if (!(*got_frame))
        return 0;

    /* prepare packet for muxing */
    enc_pkt.stream_index = stream_index;
    av_packet_rescale_ts(&enc_pkt,
                         ofmt_ctx->streams[stream_index]->codec->time_base,
                         ofmt_ctx->streams[stream_index]->time_base);

    av_log(NULL, AV_LOG_DEBUG, "Muxing frame\n");
    /* mux encoded frame */
    //enc_pkt.stream_index = 0;
    ret = av_interleaved_write_frame(ofmt_ctx, &enc_pkt);
    return ret;
}

/**
 * @brief GestionVideo::filter_encode_write_frame - encode AVFrame
 * @param frame: frame de filter
 * @param stream_index_in
 * @param stream_index_out
 * @return le statut de processus
 */
int GestionVideo::filter_encode_write_frame(AVFrame *frame, unsigned int stream_index_in, unsigned int stream_index_out)
{
    int ret = 0;
    AVFrame *filt_frame;
    //av_log(NULL, AV_LOG_INFO, "Pushing decoded frame to filters\n");
    /* push the decoded frame into the filtergraph */
    ret = av_buffersrc_add_frame_flags(filter_ctx[stream_index_out].buffersrc_ctx,
            frame, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
        return ret;
    }
    bool stop = false;
    /* pull filtered frames from the filtergraph */
    while (!stop && ret >= 0) {
        filt_frame = av_frame_alloc();
        if (!filt_frame) {
            ret = AVERROR(ENOMEM);
            stop = true;
        }
        else {
            //av_log(NULL, AV_LOG_INFO, "Pulling filtered frame from filters\n");
            ret = av_buffersink_get_frame(filter_ctx[stream_index_out].buffersink_ctx,
                    filt_frame);
            if (ret < 0) {
                /* if no more frames for output - returns AVERROR(EAGAIN)
                 * if flushed and no more frames for output - returns AVERROR_EOF
                 * rewrite retcode to 0 to show it as normal procedure completion
                 */
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                    ret = 0;
                av_frame_free(&filt_frame);
                stop = true;
            }
            else{
                filt_frame->pict_type = AV_PICTURE_TYPE_NONE;
                ret = encode_write_frame(filt_frame, stream_index_out, NULL);
            }
        }
    }

    return ret;
}

/**
 * @brief GestionVideo::flush_encoder - libérer (nettoyer) l'encodeur
 * @param stream_index
 * @return le statut de processus
 */
int GestionVideo::flush_encoder(unsigned int stream_index)
{
    int ret = 0;
    int got_frame;
    while (ret >=0) {
        av_log(NULL, AV_LOG_INFO, "Flushing stream #%u encoder\n", stream_index);
        ret = encode_write_frame(NULL, stream_index, &got_frame);
        if (!got_frame && ret >=0)
            return 0;
    }
    return ret;
}

/**
 * @brief GestionVideo::freeMemory - libérer les mémoires
 * @param packet: AVPacket
 * @param frame: AVFrame
 * @param filter_ctx: Filtering Context
 */
void GestionVideo::freeMemory(AVPacket * packet, AVFrame *frame, FilteringContext * filter_ctx)
{
    if(packet->data != NULL)
        av_packet_unref(packet);
    av_frame_free(&frame);
    if(ifmt_ctx){
        for (unsigned int i = 0; i < ifmt_ctx->nb_streams; i++) {
            avcodec_close(ifmt_ctx->streams[i]->codec);
        }
        if(ofmt_ctx != NULL){
            for (unsigned int i = 0; i < ofmt_ctx->nb_streams; i++) {
                avcodec_close(ofmt_ctx->streams[i]->codec);
            }
            /*if (freeOutput && ofmt_ctx && ofmt_ctx->nb_streams > 0 && ofmt_ctx->streams[0] && ofmt_ctx->streams[0]->codec)
                avcodec_close(ofmt_ctx->streams[0]->codec);
            */
            for (unsigned int i = 0; i < ofmt_ctx->nb_streams; i++) {
                if (filter_ctx && filter_ctx[i].filter_graph)
                    avfilter_graph_free(&filter_ctx[i].filter_graph);
            }
            av_free(filter_ctx);
        }

        avformat_close_input(&ifmt_ctx);
        ifmt_ctx = NULL;
        if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
            avio_closep(&ofmt_ctx->pb);
        if(ofmt_ctx){
            avformat_free_context(ofmt_ctx);
            ofmt_ctx = NULL;
        }

    }
}
