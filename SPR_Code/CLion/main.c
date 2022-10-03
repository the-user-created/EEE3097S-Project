#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "blowfish/blowfish.h"
#include "heatshrink/heatshrink_encoder.h"
#include "heatshrink/heatshrink_decoder.h"
#include "crc32/crc32.h"

#define KEYSIZE   32
#define DATASIZE  198

uint8_t compressed_size[4] = {0};

void DumpHex(const void* data, size_t size) {
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    for (i = 0; i < size; ++i) {
        printf("%02X ", ((unsigned char*)data)[i]);
        if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char*)data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i+1) % 8 == 0 || i+1 == size) {
            printf(" ");
            if ((i+1) % 16 == 0) {
                printf("|  %s \n", ascii);
            } else if (i+1 == size) {
                ascii[(i+1) % 16] = '\0';
                if ((i+1) % 16 <= 8) {
                    printf(" ");
                }
                for (j = (i+1) % 16; j < 16; ++j) {
                    printf("   ");
                }
                printf("|  %s \n", ascii);
            }
        }
    }
}

void Compress(uint8_t *input, uint32_t input_size, uint8_t *output)
{
    static heatshrink_encoder hse;

    heatshrink_encoder_reset(&hse);
    size_t comp_sz = input_size + (input_size/2) + 4;
    printf("%lu\n", sizeof(comp_sz));
    uint8_t *comp = calloc(input_size, sizeof(uint8_t));

    size_t count = 0;
    uint32_t sunk = 0;
    uint32_t polled = 0;
    while (sunk < input_size) {
        heatshrink_encoder_sink(&hse, &input[sunk], input_size - sunk, &count);
        sunk += count;
        if (sunk == input_size) {
            heatshrink_encoder_finish(&hse);
        }

        HSE_poll_res pres;
        do {
            pres = heatshrink_encoder_poll(&hse, &comp[polled], comp_sz - polled, &count);
            polled += count;
        } while (pres == HSER_POLL_MORE);
        if (sunk == input_size) {
            heatshrink_encoder_finish(&hse);
        }
    }

    //uint32_t val = polled;
    /*for (int i = 0; i < 4; i++) {
        if (val + DATASIZE < DATASIZE) {
            compressed_size[i] = (uint8_t) DATASIZE;
            val = val - DATASIZE;
        } else {
            compressed_size[i] = val;
            val = 0;
        }
    }*/

    memmove(compressed_size, &polled, sizeof(uint32_t));

    memcpy(output, comp, polled);
    free(comp);
}

void Decompress(uint8_t *input, uint32_t input_size, uint32_t comp_size, uint8_t *output)
{
    static heatshrink_decoder hsd;
    heatshrink_decoder_reset(&hsd);

    size_t decomp_sz = input_size + (input_size/2) + 4;
    uint8_t *decomp = calloc(input_size, sizeof(uint8_t));
    uint32_t sunk = 0;
    uint32_t polled = 0;
    size_t count = 0;

    while (sunk < comp_size) {
        heatshrink_decoder_sink(&hsd, &input[sunk], comp_size - sunk, &count);
        sunk += count;
        if (sunk == comp_size) {
            heatshrink_decoder_finish(&hsd);
        }

        HSD_poll_res pres;
        do {
            pres = heatshrink_decoder_poll(&hsd, &decomp[polled],decomp_sz - polled, &count);
            polled += count;
        } while (pres == HSDR_POLL_MORE);
        if (sunk == comp_size) {
            heatshrink_decoder_finish(&hsd);
        }
    }

    memcpy(output, decomp, polled);
    free(decomp);
}

uint8_t Encrypt(uint8_t *input, uint32_t input_size, uint8_t *output) {
    uint8_t padding = input_size%8;  // Get trailing bytes

    // Determine how many bytes should be added to allow for complete encryption
    if (padding != 0) {
        padding = 8 - padding;
    }

    // Adding padding to the data
    uint8_t *temp_input = calloc(input_size + padding, sizeof(uint8_t));
    memmove(temp_input, input, input_size);

    uint8_t *encrypted = calloc(input_size + padding, sizeof(uint8_t));
    blowfish_encrypt(temp_input, input_size + padding, encrypted);
    free(temp_input);

    memmove(output, encrypted, input_size + padding);
    free(encrypted);

    return padding;
}

void Decrypt(uint8_t *input, uint32_t input_size, uint8_t padding, uint8_t *output) {
    uint8_t *decrypted = calloc(input_size, sizeof(uint8_t));
    blowfish_decrypt(input, input_size, decrypted);
    memmove(output, decrypted, input_size + padding);
    free(decrypted);
}

int main()
{
    uint8_t *arr1 = calloc(DATASIZE, sizeof(uint8_t));
    for (int i = 0 ; i < DATASIZE ; i++) { arr1[i] = rand()%255; }

    DumpHex(arr1, 198);

    for (int i = 0; i < DATASIZE; i++) {
        arr1[i] = arr1[i] + rand()%5;
    }

    DumpHex(arr1, 198);

    /*uint8_t arr[8] = {0};
    uint32_t time_val = time(0);

    memmove(arr, &time_val, sizeof(uint32_t));

    DumpHex(arr, 8);

    printf("%d\n", arr[0] + (arr[1] << 8) + (arr[2] << 16) + (arr[3] << 24));

    printf("%u\n", time_val);

    printf("%d,%d\n", (time_val >> 8) & 0x00FF, time_val & 0x00FF);

    //uint8_t data[16] = {0x3f, 0xfc, 0x0b, 0x03, 0x02, 0xfe, 0x12, 0xfc, 0xe9, 0x80, 0xff, 0x17, 0x03, 0x22, 0xfe, 0x24};
    //DumpHex(data, 16);

    //uint32_t crc2 = 0;
    //crc2 = xcrc32(data, sizeof(data), crc2);
    //printf("crc2: %u\n", crc2);*/

    /*int16_t magn[3] = {0};
    int16_t accel[3] = {0}, gyro[3] = {0};
    //char buffer[50];
    //snprintf(buffer, sizeof(buffer), "\r%d,%d,%d,%d,%d,%d,%d,%d,%d", accel[0], accel[1], accel[2], gyro[0], gyro[1], gyro[2], magn[0], magn[1], magn[2]);

    accel[0] = 13905;
    accel[1] = 2048;
    accel[2] = 7784;
    printf("%d,%d,%d\n", accel[0], accel[1], accel[2]);

    uint8_t buffer2[6] = {0};
    buffer2[0] = (accel[0] >> 8) & 0x00FF;
    buffer2[1] = accel[0] & 0x00FF;
    buffer2[2] = (accel[1] >> 8) & 0x00FF;
    buffer2[3] = accel[1] & 0x00FF;
    buffer2[4] = (accel[2] >> 8) & 0x00FF;
    buffer2[5] = accel[2] & 0x00FF;

    printf("%d,%d,%d,%d,%d,%d\n", buffer2[0], buffer2[1], buffer2[2], buffer2[3], buffer2[4], buffer2[5]);

    printf("%d,%d,%d\n", buffer2[0] << 8 | buffer2[1], buffer2[2] << 8 | buffer2[3], buffer2[4] << 8 | buffer2[5]);*/

    /*uint8_t *arr1 = calloc(DATASIZE, sizeof(uint8_t));
    uint8_t *arr2 = calloc(DATASIZE, sizeof(uint8_t));
    arr1[55] = 0x11;
    arr1[33] = 0x01;
    arr1[12] = 0x45;
    arr1[56] = 0x23;
    arr1[42] = 0x77;
    arr1[44] = 0x65;

    for (int i = 0 ; i < DATASIZE ; i++) { arr1[i] = rand()%30; }

    uint32_t comp_size;
    uint8_t padding_len[1] = {0};
    uint8_t *output = calloc(4, sizeof(uint8_t));

    uint32_t *key = calloc(KEYSIZE, sizeof(uint32_t));
    key[0] = 0x11;
    key[31] = 0xFF;

    blowfish_init(key, KEYSIZE);
    free(key);

    xcrc32(arr1, DATASIZE, output);
    DumpHex(output, 4);

    //DumpHex(arr1, DATASIZE);
    //printf("\n\n\n");
    Compress(arr1, DATASIZE, arr2);

    comp_size = compressed_size[0] + (compressed_size[1] << 8) + (compressed_size[2] << 16) + (compressed_size[3] << 24);
    //DumpHex(arr2, comp_size);
    //printf("\n");

    padding_len[0] = Encrypt(arr2, comp_size, arr1);
    //DumpHex(arr1, comp_size + padding_len[0]);
    //printf("\n");

    Decrypt(arr1, comp_size, padding_len[0], arr2);
    //DumpHex(arr2, comp_size);
    Decompress(arr2, DATASIZE, comp_size, arr1);
    //DumpHex(arr2, DATASIZE);
    //printf("\n");
    memset(output, 0, 4);

    xcrc32(arr1, DATASIZE, output);
    DumpHex(output, 4);

    free(arr1);
    free(arr2);
    free(output);*/

    return 0;
}
