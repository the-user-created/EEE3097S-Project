#include "crc32.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/*
@deftypefn Extension {unsigned int} crc32 (const unsigned char *@var{buf}, @
  int @var{len}, unsigned int @var{init})

Compute the 32-bit CRC of @var{buf} which has length @var{len}.  The
starting value is @var{init}; this may be used to compute the CRC of
data split across multiple buffers by passing the return value of each
call as the @var{init} parameter of the next.

This is used by the @command{gdb} remote protocol for the @samp{qCRC}
command.  In order to get the same results as gdb for a block of data,
you must pass the first CRC parameter as @code{0xffffffff}.

This CRC can be specified as:

  Width  : 32
  Poly   : 0x04c11db7
  Init   : parameter, typically 0xffffffff
  RefIn  : false
  RefOut : false
  XorOut : 0

This differs from the "standard" CRC-32 algorithm in that the values
are not reflected, and there is no final XOR value.  These differences
make it easy to compose the values of multiple blocks.

@end deftypefn
*/

void xcrc32 (const uint8_t *buf, uint32_t len, uint8_t *output)
{
    uint32_t crc = 0;
    while (len--)
    {
        crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *buf) & 255];
        buf++;
    }

    memmove(output, &crc, sizeof(uint32_t));
}
