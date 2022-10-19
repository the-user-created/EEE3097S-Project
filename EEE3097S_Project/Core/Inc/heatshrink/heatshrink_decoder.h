#ifndef HEATSHRINK_DECODER_H
#define HEATSHRINK_DECODER_H

#include <stdint.h>
#include <stddef.h>
#include "heatshrink_common.h"
#include "heatshrink_config.h"

typedef enum {
    HSDR_SINK_OK,
    HSDR_SINK_FULL,
    HSDR_SINK_ERROR_NULL=-1,
} HSD_sink_res;

typedef enum {
    HSDR_POLL_EMPTY,
    HSDR_POLL_MORE,
    HSDR_POLL_ERROR_NULL=-1,
    HSDR_POLL_ERROR_UNKNOWN=-2,
} HSD_poll_res;

typedef enum {
    HSDR_FINISH_DONE,
    HSDR_FINISH_MORE,
    HSDR_FINISH_ERROR_NULL=-1,
} HSD_finish_res;

#define HEATSHRINK_DECODER_INPUT_BUFFER_SIZE(_) HEATSHRINK_STATIC_INPUT_BUFFER_SIZE
#define HEATSHRINK_DECODER_WINDOW_BITS(_) (HEATSHRINK_STATIC_WINDOW_BITS)
#define HEATSHRINK_DECODER_LOOKAHEAD_BITS(BUF) (HEATSHRINK_STATIC_LOOKAHEAD_BITS)

typedef struct {
    uint16_t input_size;
    uint16_t input_index;
    uint16_t output_count;
    uint16_t output_index;
    uint16_t head_index;
    uint8_t state;
    uint8_t current_byte;
    uint8_t bit_index;
    uint8_t buffers[(1 << HEATSHRINK_DECODER_WINDOW_BITS(_)) + HEATSHRINK_DECODER_INPUT_BUFFER_SIZE(_)];
} heatshrink_decoder;

void heatshrink_decoder_reset(heatshrink_decoder *hsd);
HSD_sink_res heatshrink_decoder_sink(heatshrink_decoder *hsd, uint8_t *in_buf, size_t size, size_t *input_size);
HSD_poll_res heatshrink_decoder_poll(heatshrink_decoder *hsd, uint8_t *out_buf, size_t out_buf_size, size_t *output_size);
HSD_finish_res heatshrink_decoder_finish(heatshrink_decoder *hsd);

#endif
