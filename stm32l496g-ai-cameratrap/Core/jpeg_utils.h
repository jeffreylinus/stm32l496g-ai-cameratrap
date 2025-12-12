/*
 * jpeg_utils.h
 *
 *  Created on: Nov 26, 2025
 *      Author: Sumeeth
 */

#ifndef JPEG_UTILS_H
#define JPEG_UTILS_H

#include <stdint.h>

int jpeg_encode_gray(const uint8_t *src,
                     int            width,
                     int            height,
                     int            quality,   // 1, 2, or 3
                     uint8_t       *dst,
                     uint32_t       dst_max);

#endif
