#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <linux/videodev.h>
#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/time.h>
#include <getopt.h>
#include <string.h>

#include "mpegenc.h"

AVFormat *first_format;
/* XXX: suppress it ! */
int data_out_size;

const char *comment_string = 
"+title=Test Video +author=FFMpeg +copyright=Free +comment=Generated by FFMpeg 1.0";

void register_avformat(AVFormat *format)
{
    AVFormat **p;
    p = &first_format;
    while (*p != NULL) p = &(*p)->next;
    *p = format;
    format->next = NULL;
}

AVFormat *guess_format(const char *short_name, const char *filename, const char *mime_type)
{
    AVFormat *fmt, *fmt_found;
    int score_max, score;
    const char *ext, *p;
    char ext1[32], *q;

    /* find the proper file type */
    fmt_found = NULL;
    score_max = 0;
    fmt = first_format;
    while (fmt != NULL) {
        score = 0;
        if (fmt->name && short_name && !strcmp(fmt->name, short_name))
            score += 100;
        if (fmt->mime_type && mime_type && !strcmp(fmt->mime_type, mime_type))
            score += 10;
        if (filename && fmt->extensions) {
            ext = strrchr(filename, '.');
            if (ext) {
                ext++;
                p = fmt->extensions;
                for(;;) {
                    q = ext1;
                    while (*p != '\0' && *p != ',') 
                        *q++ = *p++;
                    *q = '\0';
                    if (!strcasecmp(ext1, ext)) {
                        score += 5;
                        break;
                    }
                    if (*p == '\0') 
                        break;
                    p++;
                }
            }
        }
        if (score > score_max) {
            score_max = score;
            fmt_found = fmt;
        }
        fmt = fmt->next;
    }
    return fmt_found;
}   

/* return TRUE if val is a prefix of str. If it returns TRUE, ptr is
   set to the next character in 'str' after the prefix */
int strstart(const char *str, const char *val, const char **ptr)
{
    const char *p, *q;
    p = str;
    q = val;
    while (*q != '\0') {
        if (*p != *q)
            return 0;
        p++;
        q++;
    }
    if (ptr)
        *ptr = p;
    return 1;
}

/* simple formats */
int raw_write_header(struct AVFormatContext *s)
{
    return 0;
}

int raw_write_audio(struct AVFormatContext *s, 
                    unsigned char *buf, int size)
{
    put_buffer(&s->pb, buf, size);
    put_flush_packet(&s->pb);
    return 0;
}

int raw_write_video(struct AVFormatContext *s, 
                    unsigned char *buf, int size)
{
    put_buffer(&s->pb, buf, size);
    put_flush_packet(&s->pb);
    return 0;
}

int raw_write_trailer(struct AVFormatContext *s)
{
    return 0;
}

AVFormat mp2_format = {
    "mp2",
    "MPEG audio layer 2",
    "audio/x-mpeg",
    "mp2,mp3",
    CODEC_ID_MP2,
    0,
    raw_write_header,
    raw_write_audio,
    NULL,
    raw_write_trailer,
};

AVFormat ac3_format = {
    "ac3",
    "raw ac3",
    "audio/x-ac3", 
    "ac3",
    CODEC_ID_AC3,
    0,
    raw_write_header,
    raw_write_audio,
    NULL,
    raw_write_trailer,
};

AVFormat h263_format = {
    "h263",
    "raw h263",
    "video/x-h263",
    "h263",
    0,
    CODEC_ID_H263,
    raw_write_header,
    NULL,
    raw_write_video,
    raw_write_trailer,
};

AVFormat mpeg1video_format = {
    "mpeg1video",
    "MPEG1 video",
    "video/mpeg",
    "mpg,mpeg",
    0,
    CODEC_ID_MPEG1VIDEO,
    raw_write_header,
    NULL,
    raw_write_video,
    raw_write_trailer,
};

/* encoder management */
AVEncoder *first_encoder;

void register_avencoder(AVEncoder *format)
{
    AVEncoder **p;
    p = &first_encoder;
    while (*p != NULL) p = &(*p)->next;
    *p = format;
    format->next = NULL;
}

int avencoder_open(AVEncodeContext *avctx, AVEncoder *codec)
{
    int ret;

    avctx->codec = codec;
    avctx->frame_number = 0;
    avctx->priv_data = malloc(codec->priv_data_size);
    if (!avctx->priv_data) 
        return -ENOMEM;
    memset(avctx->priv_data, 0, codec->priv_data_size);
    ret = avctx->codec->init(avctx);
    if (ret < 0) {
        free(avctx->priv_data);
        avctx->priv_data = NULL;
        return ret;
    }
    return 0;
}

int avencoder_encode(AVEncodeContext *avctx, UINT8 *buf, int buf_size, void *data)
{
    int ret;

    ret = avctx->codec->encode(avctx, buf, buf_size, data);
    avctx->frame_number++;
    return ret;
}

int avencoder_close(AVEncodeContext *avctx)
{
    if (avctx->codec->close)
        avctx->codec->close(avctx);
    free(avctx->priv_data);
    avctx->priv_data = NULL;
    return 0;
}

AVEncoder *avencoder_find(enum CodecID id)
{
    AVEncoder *p;
    p = first_encoder;
    while (p) {
        if (p->id == id)
            return p;
        p = p->next;
    }
    return NULL;
}


void avencoder_string(char *buf, int buf_size, AVEncodeContext *enc)
{
    switch(enc->codec->type) {
    case CODEC_TYPE_VIDEO:
        snprintf(buf, buf_size,
                 "Video: %s, %dx%d, %d fps, %d kb/s",
                 enc->codec->name, enc->width, enc->height, enc->rate, enc->bit_rate / 1000);
        break;
    case CODEC_TYPE_AUDIO:
        snprintf(buf, buf_size,
                 "Audio: %s, %d Hz, %s, %d kb/s",
                 enc->codec->name, enc->rate,
                 enc->channels == 2 ? "stereo" : "mono", 
                 enc->bit_rate / 1000);
        break;
    default:
        abort();
    }
}

/* PutByteFormat */

int init_put_byte(PutByteContext *s,
                  unsigned char *buffer,
                  int buffer_size,
                  void *opaque,
                  void (*write_packet)(void *opaque, UINT8 *buf, int buf_size),
                  int (*write_seek)(void *opaque, long long offset, int whence))
{
    s->buffer = buffer;
    s->buf_ptr = buffer;
    s->buf_end = buffer + buffer_size;
    s->opaque = opaque;
    s->write_packet = write_packet;
    s->write_seek = write_seek;
    s->pos = 0;
    return 0;
}
                  

static void flush_buffer(PutByteContext *s)
{
    if (s->buf_ptr > s->buffer) {
        if (s->write_packet)
            s->write_packet(s->opaque, s->buffer, s->buf_ptr - s->buffer);
        s->pos += s->buf_ptr - s->buffer;
    }
    s->buf_ptr = s->buffer;
}

void put_byte(PutByteContext *s, int b)
{
    *(s->buf_ptr)++ = b;
    if (s->buf_ptr >= s->buf_end) 
        flush_buffer(s);
}

void put_buffer(PutByteContext *s, unsigned char *buf, int size)
{
    int len;

    while (size > 0) {
        len = (s->buf_end - s->buf_ptr);
        if (len > size)
            len = size;
        memcpy(s->buf_ptr, buf, len);
        s->buf_ptr += len;

        if (s->buf_ptr >= s->buf_end) 
            flush_buffer(s);

        buf += len;
        size -= len;
    }
}

void put_flush_packet(PutByteContext *s)
{
    flush_buffer(s);
}

/* XXX: this seek is not correct if we go after the end of the written data */
long long put_seek(PutByteContext *s, long long offset, int whence)
{
    long long offset1;

    if (whence != SEEK_CUR && whence != SEEK_SET)
        return -1;
    if (whence == SEEK_CUR)
        offset += s->pos + s->buf_ptr - s->buffer;
    
    offset1 = offset - s->pos;
    if (offset1 >= 0 && offset1 < (s->buf_end - s->buffer)) {
        /* can do the seek inside the buffer */
        s->buf_ptr = s->buffer + offset1;
    } else {
        if (!s->write_seek)
            return -1;
        flush_buffer(s);
        s->write_seek(s->opaque, offset, whence);
    }
    return offset;
}

long long put_pos(PutByteContext *s)
{
    return put_seek(s, 0, SEEK_CUR);
}

void put_le32(PutByteContext *s, unsigned int val)
{
    put_byte(s, val);
    put_byte(s, val >> 8);
    put_byte(s, val >> 16);
    put_byte(s, val >> 24);
}

void put_le64(PutByteContext *s, unsigned long long val)
{
    put_le32(s, val & 0xffffffff);
    put_le32(s, val >> 32);
}

void put_le16(PutByteContext *s, unsigned int val)
{
    put_byte(s, val);
    put_byte(s, val >> 8);
}

void put_tag(PutByteContext *s, char *tag)
{
    while (*tag) {
        put_byte(s, *tag++);
    }
}

