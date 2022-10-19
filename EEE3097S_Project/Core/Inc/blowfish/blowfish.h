#ifndef __BLOWFISH__
#define __BLOWFISH__

uint32_t f(uint32_t x);
void encrypt(uint32_t *left, uint32_t *right);
void decrypt(uint32_t *left, uint32_t *right);
void blowfish_init(const uint32_t *key, uint8_t key_len);
void blowfish_encrypt(uint8_t data[], uint32_t data_size, uint8_t *output);
void blowfish_decrypt(uint8_t data[], uint32_t data_size, uint8_t *output);

#endif
