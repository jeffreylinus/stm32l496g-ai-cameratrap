/*
 * jpeg_utils.c
 *
 *  Created on: Nov 26, 2025
 *      Author: Sumeeth
 */

#include <stdint.h>
#include <string.h>

//
// TinyJPEG expects some platform to define tje_log() for debug builds.
// On bare-metal STM32 we don't want logging, so provide a no-op stub.
//
void tje_log(const char *msg) { (void)msg; }

#define TJE_IMPLEMENTATION
#include "tiny_jpeg.h"       // from TinyJPEG repo

#include "jpeg_utils.h"

// Max crop size you will ever encode (your frame is 160x120)
#define JPEG_MAX_W 160
#define JPEG_MAX_H 120

// TinyJPEG only encodes RGB/RGBA; expand grayscale to RGB.
static uint8_t g_rgb_buf[JPEG_MAX_W * JPEG_MAX_H * 3];

typedef struct {
    uint8_t *dst;
    uint32_t max;
    uint32_t pos;
    uint8_t  overflow;
} JpegBufCtx;

// TinyJPEG callback: called whenever there are 'size' bytes ready
static void stm_jpeg_write(void *ctx_void, void *data, int size)
{
    JpegBufCtx *ctx = (JpegBufCtx *)ctx_void;
    if (ctx->overflow) {
        return;
    }

    uint32_t sz = (uint32_t)size;
    if (ctx->pos + sz > ctx->max) {
        uint32_t can = (ctx->max > ctx->pos) ? (ctx->max - ctx->pos) : 0;
        if (can > 0) {
            memcpy(ctx->dst + ctx->pos, data, can);
            ctx->pos += can;
        }
        ctx->overflow = 1;
        return;
    }

    memcpy(ctx->dst + ctx->pos, data, sz);
    ctx->pos += sz;
}

int jpeg_encode_gray(const uint8_t *src,
                     int            width,
                     int            height,
                     int            quality,
                     uint8_t       *dst,
                     uint32_t       dst_max)
{
    // Sanity: crop must fit our static buffer
    if (width  > JPEG_MAX_W ||
        height > JPEG_MAX_H ||
        width <= 0 || height <= 0) {
        return -1;
    }

    const int num_pixels = width * height;

    // Expand grayscale [0..255] → interleaved RGB triplets
    for (int i = 0; i < num_pixels; ++i) {
        uint8_t g = src[i];
        int j = i * 3;
        g_rgb_buf[j + 0] = g;
        g_rgb_buf[j + 1] = g;
        g_rgb_buf[j + 2] = g;
    }

    JpegBufCtx ctx;
    ctx.dst      = dst;
    ctx.max      = dst_max;
    ctx.pos      = 0;
    ctx.overflow = 0;

    // TinyJPEG quality must be 1, 2, or 3
    if (quality < 1) quality = 1;
    if (quality > 3) quality = 3;

    int ok = tje_encode_with_func(
                 stm_jpeg_write,
                 &ctx,
                 quality,
                 width,
                 height,
                 3,             // 3 components → RGB
                 g_rgb_buf);

    if (!ok || ctx.overflow) {
        return -1;
    }

    return (int)ctx.pos;      // number of bytes written into dst
}


