#include <stdint.h>
typedef uint8_t     Twofish_Byte;
typedef uint32_t    Twofish_UInt32;
typedef uint64_t    Twofish_UInt64;
typedef struct {
  Twofish_UInt32 s[4][256];   
  Twofish_UInt32 K[40];       
} Twofish_key;
typedef enum {
  Twofish_option_CBC,
  Twofish_option_PaddingPKCS7,
  Twofish_options_default = Twofish_option_CBC | Twofish_option_PaddingPKCS7
} Twofish_options;
typedef struct {
  Twofish_Byte iv[16];
  Twofish_key key;
  Twofish_options options;
} Twofish_context;
extern void Twofish_initialise(void);
extern void Twofish_prepare_key( Twofish_Byte key[], int key_len, Twofish_key * xkey );
extern void Twofish_encrypt_block(Twofish_key * xkey, Twofish_Byte p[16], Twofish_Byte c[16]);
extern void Twofish_encrypt(Twofish_context *context, Twofish_Byte *input, Twofish_UInt64 input_length, Twofish_Byte *output, Twofish_UInt64 output_length);
extern Twofish_UInt64 Twofish_get_output_length(Twofish_context *context, Twofish_UInt64 input_lenght);
extern void Twofish_decrypt_block(Twofish_key * xkey, Twofish_Byte c[16], Twofish_Byte p[16]);
extern void Twofish_decrypt(Twofish_context *context, Twofish_Byte *input, Twofish_UInt64 input_length, Twofish_Byte *output, Twofish_UInt64 *output_length);
extern void Twofish_setup(Twofish_context *context, Twofish_Byte key[32], Twofish_Byte iv[16], Twofish_options options);
