#include <stdlib.h>
#include <string.h>
#include "blowfish/constants.h"

#define SWAP(x, y, temp) {temp = (x); (x) = (y); (y) = temp;}

uint32_t f(uint32_t x) {
    uint32_t h = S[x >> 24] + S[x >> 16 & 0xff];
    return ( h ^ S[x >> 8 & 0xff] ) + S[x & 0xff];
}

void encrypt(uint32_t *left, uint32_t *right) {
    uint32_t temp;
    for (short r = 0; r < 16; r++) {
        *left = *left ^ P[r];
        *right = f(*left) ^ *right;
        SWAP(*left, *right, temp)
    }
    SWAP(*left, *right, temp)
    *right = *right ^ P[16];
    *left = *left ^ P[17];
}

void decrypt(uint32_t *left, uint32_t *right) {
    uint32_t temp;
    for (short r = 17; r > 1; r--) {
        *left = *left ^ P[r];
        *right = f(*left) ^ *right;
        SWAP(*left, *right, temp)
    }
    SWAP(*left, *right, temp)
    *right = *right ^ P[1];
    *left = *left ^ P[0];
}

void blowfish_init(const uint32_t *key, uint8_t key_len) {
    // initialize P box w/ key
    uint32_t k;

    for (short i = 0, p = 0; i < 18; i++) {
        k = 0x00;
        for (short j = 0; j < 4; j++) {
            k = (k << 8) | (uint8_t) key[p];
            p = (p + 1) % key_len;
        }
        P[i] ^= k;
    }

    // blowfish key expansion (521 iterations)
    uint32_t l = 0x00, r = 0x00;
    for (short i = 0; i < 18; i+=2) {
        encrypt(&l, &r);
        P[i] = l;
        P[i+1] = r;
    }

    for (short j = 0; j < 256; j+=2) {
        encrypt(&l, &r);
        S[j] = l;
        S[j+1] = r;
    }
}

void blowfish_encrypt(uint8_t data[], uint32_t data_size, uint8_t *output)
{
    uint8_t *encrypted = calloc(data_size, sizeof(uint8_t));
    uint32_t i;
    uint32_t left, right;
    uint64_t chunk;

    for (i = 0; i < data_size; i += 8) {
        /* make 8 byte chunks */
        chunk = 0x00;
        memmove(&chunk, data + i, sizeof(chunk));

        /* split into two 4 byte chunks */
        left = right = 0x00;
        left   = (uint32_t)(chunk >> 32);
        right  = (uint32_t)(chunk);

        encrypt(&left, &right);

        /* merge encrypted halves into a single 8 byte chunk again */
        chunk = 0x00;
        chunk |= left; chunk <<= 32;
        chunk |= right;

        /* append the chunk into the answer */
        memmove(encrypted + i, &chunk, sizeof(chunk));
    }

    memmove(output, encrypted, data_size);
    free(encrypted);
}

void blowfish_decrypt(uint8_t data[], uint32_t data_size, uint8_t *output)
{
    uint8_t *decrypted = calloc(data_size, sizeof(uint8_t));
    uint32_t i;
    uint32_t left, right;
    uint64_t chunk;

    for (i = 0; i < data_size; i += 8) {
        chunk = 0x00;
        memmove(&chunk, data + i, sizeof(chunk));

        left = right = 0x00;
        left   = (uint32_t)(chunk >> 32);
        right  = (uint32_t)(chunk);

        decrypt(&left, &right);

        chunk = 0x00;
        chunk |= left; chunk <<= 32;
        chunk |= right;

        memmove(decrypted + i, &chunk, sizeof(chunk));
    }

    memmove(output, decrypted, data_size);
    free(decrypted);
}

