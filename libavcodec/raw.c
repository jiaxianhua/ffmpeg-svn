/*
 * Raw Video Codec
 * Copyright (c) 2001 Fabrice Bellard.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
/**
 * @file raw.c
 * Raw Video Codec
 */
 
#include "avcodec.h"


typedef struct PixleFormatTag {
    int pix_fmt;
    unsigned int fourcc;
} PixelFormatTag;

const PixelFormatTag pixelFormatTags[] = {
    { PIX_FMT_YUV422, MKTAG('Y', '4', '2', '2') },
    { PIX_FMT_YUV420P, MKTAG('I', '4', '2', '0') },
    { -1, 0 },
};

static int findPixelFormat(unsigned int fourcc)
{
    const PixelFormatTag * tags = pixelFormatTags;
    while (tags->pix_fmt >= 0) {
        if (tags->fourcc == fourcc)
            return tags->pix_fmt;
        tags++;
    }
    return PIX_FMT_YUV420P;
}


typedef struct RawVideoContext {
    unsigned char * buffer;  /* block of memory for holding one frame */
    unsigned char * p;       /* current position in buffer */
    int             length;  /* number of bytes in buffer */
} RawVideoContext;


static int raw_init(AVCodecContext *avctx)
{
    RawVideoContext *context = avctx->priv_data;

    if (avctx->codec_tag) {
	    avctx->pix_fmt = findPixelFormat(avctx->codec_tag);
    }

	context->length = avpicture_get_size(avctx->pix_fmt, avctx->width, avctx->height);
	context->buffer = av_malloc(context->length);
	context->p      = context->buffer;

    if (! context->buffer) {
        return -1;
    }

    return 0;
}

static int raw_decode(AVCodecContext *avctx,
			    void *data, int *data_size,
			    uint8_t *buf, int buf_size)
{
    RawVideoContext *context = avctx->priv_data;
    int bytesNeeded;

    AVPicture * picture = (AVPicture *) data;

    /* Early out without copy if packet size == frame size */
    if (buf_size == context->length  &&  context->p == context->buffer) {
        avpicture_fill(picture, buf, avctx->pix_fmt, avctx->width, avctx->height);
        *data_size = sizeof(AVPicture);
        return buf_size;
    }

    bytesNeeded = context->length - (context->p - context->buffer);
    if (buf_size < bytesNeeded) {
        memcpy(context->p, buf, buf_size);
        context->p += buf_size;
        *data_size = 0;
        return buf_size;
    }

    memcpy(context->p, buf, bytesNeeded);
    context->p = context->buffer;
    avpicture_fill(picture, context->buffer, avctx->pix_fmt, avctx->width, avctx->height);
    *data_size = sizeof(AVPicture);
    return bytesNeeded;
}

static int raw_close(AVCodecContext *avctx)
{
    RawVideoContext *context = avctx->priv_data;

    av_freep(& context->buffer);

    return 0;
}

static int raw_encode(AVCodecContext *avctx,
			    unsigned char *frame, int buf_size, void *data)
{
	AVPicture * picture = data;

    unsigned char *src;
	unsigned char *dest = frame;
    int i, j;

	int w = avctx->width;
	int h = avctx->height;
	int size = avpicture_get_size(avctx->pix_fmt, w, h);

    if (size > buf_size) {
        return -1;
    }

    switch(avctx->pix_fmt) {
    case PIX_FMT_YUV420P:
        for(i=0;i<3;i++) {
            if (i == 1) {
                w >>= 1;
                h >>= 1;
            }
            src = picture->data[i];
            for(j=0;j<h;j++) {
                memcpy(dest, src, w);
                dest += w;
                src += picture->linesize[i];
            }
        }
        break;
    case PIX_FMT_YUV422P:
        for(i=0;i<3;i++) {
            if (i == 1) {
                w >>= 1;
            }
            src = picture->data[i];
            for(j=0;j<h;j++) {
                memcpy(dest, src, w);
                dest += w;
                src += picture->linesize[i];
            }
        }
        break;
    case PIX_FMT_YUV444P:
        for(i=0;i<3;i++) {
            src = picture->data[i];
            for(j=0;j<h;j++) {
                memcpy(dest, src, w);
                dest += w;
                src += picture->linesize[i];
            }
        }
        break;
    case PIX_FMT_YUV422:
        src = picture->data[0];
        for(j=0;j<h;j++) {
            memcpy(dest, src, w * 2);
            dest += w * 2;
            src += picture->linesize[0];
        }
        break;
    case PIX_FMT_RGB24:
    case PIX_FMT_BGR24:
        src = picture->data[0];
        for(j=0;j<h;j++) {
            memcpy(dest, src, w * 3);
            dest += w * 3;
            src += picture->linesize[0];
        }
        break;
    default:
        return -1;
    }

    return size;
}


AVCodec rawvideo_codec = {
    "rawvideo",
    CODEC_TYPE_VIDEO,
    CODEC_ID_RAWVIDEO,
    sizeof(RawVideoContext),
    raw_init,
    raw_encode,
    raw_close,
    raw_decode,
};
