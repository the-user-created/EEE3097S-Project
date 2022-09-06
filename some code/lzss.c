#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "lzlocal.h"
#include "bitfile/bitfile.h"
static buffers_t buffers;
int EncodeLZSS(FILE *fpIn, FILE *fpOut)
{
    bit_file_t *bfpOut;
    encoded_string_t matchData;
    int c;
    unsigned int i;
    unsigned int len;                       
    unsigned int windowHead, uncodedHead;
    if ((NULL == fpIn) || (NULL == fpOut))
    {
        errno = ENOENT;
        return -1;
    }
    bfpOut = MakeBitFile(fpOut, BF_WRITE);
    if (NULL == bfpOut)
    {
        perror("Making Output File a BitFile");
        return -1;
    }
    windowHead = 0;
    uncodedHead = 0;
    memset(buffers.slidingWindow, ' ', WINDOW_SIZE * sizeof(unsigned char));
    for (len = 0; len < MAX_CODED && (c = getc(fpIn)) != EOF; len++)
    {
        buffers.uncodedLookahead[len] = c;
    }
    if (0 == len)
    {
        return 0;   
    }
    i = InitializeSearchStructures(&buffers);
    if (0 != i)
    {
        return i;       
    }
    while (len > 0)
    {
        matchData = FindMatch(&buffers, windowHead, uncodedHead, len);
        if (matchData.length > len)
        {
            matchData.length = len;
        }
        if (matchData.length <= MAX_UNCODED)
        {
            BitFilePutBit(UNCODED, bfpOut);
            BitFilePutChar(buffers.uncodedLookahead[uncodedHead], bfpOut);
            matchData.length = 1;   
        }
        else
        {
            unsigned int adjustedLen;
            adjustedLen = matchData.length - (MAX_UNCODED + 1);
            BitFilePutBit(ENCODED, bfpOut);
            BitFilePutBitsNum(bfpOut, &matchData.offset, OFFSET_BITS,
                              sizeof(unsigned int));
            BitFilePutBitsNum(bfpOut, &adjustedLen, LENGTH_BITS,
                              sizeof(unsigned int));
        }
        i = 0;
        while ((i < matchData.length) && ((c = getc(fpIn)) != EOF))
        {
            ReplaceChar(buffers.slidingWindow, windowHead,
                        buffers.uncodedLookahead[uncodedHead]);
            buffers.uncodedLookahead[uncodedHead] = c;
            CyclicInc(windowHead, WINDOW_SIZE);
            CyclicInc(uncodedHead, MAX_CODED);
            i++;
        }
        while (i < matchData.length)
        {
            ReplaceChar(buffers.slidingWindow,
                        windowHead,
                        buffers.uncodedLookahead[uncodedHead]);
            CyclicInc(windowHead, WINDOW_SIZE);
            CyclicInc(uncodedHead, MAX_CODED);
            len--;
            i++;
        }
    }
    BitFileToFILE(bfpOut);
    return 0;
}
int DecodeLZSS(FILE *fpIn, FILE *fpOut)
{
    bit_file_t *bfpIn;
    int c;
    unsigned int nextChar;
    if ((NULL == fpIn) || (NULL == fpOut))
    {
        errno = ENOENT;
        return -1;
    }
    bfpIn = MakeBitFile(fpIn, BF_READ);
    if (NULL == bfpIn)
    {
        perror("Making Input File a BitFile");
        return -1;
    }
    memset(buffers.slidingWindow, ' ', WINDOW_SIZE * sizeof(unsigned char));
    nextChar = 0;
    while (1)
    {
        if ((c = BitFileGetBit(bfpIn)) == EOF)
        {
            break;
        }
        if (c == UNCODED)
        {
            if ((c = BitFileGetChar(bfpIn)) == EOF)
            {
                break;
            }
            putc(c, fpOut);
            buffers.slidingWindow[nextChar] = c;
            CyclicInc(nextChar, WINDOW_SIZE);
        }
        else
        {
            unsigned int i;
            encoded_string_t code;
            code.offset = 0;
            code.length = 0;
            if ((BitFileGetBitsNum(bfpIn, &code.offset, OFFSET_BITS,
                                   sizeof(unsigned int))) == EOF)
            {
                break;
            }
            if ((BitFileGetBitsNum(bfpIn, &code.length, LENGTH_BITS,
                                   sizeof(unsigned int))) == EOF)
            {
                break;
            }
            code.length += MAX_UNCODED + 1;
            for (i = 0; i < code.length; i++)
            {
                c = buffers.slidingWindow[Wrap((code.offset + i), WINDOW_SIZE)];
                putc(c, fpOut);
                buffers.uncodedLookahead[i] = c;
            }
            for (i = 0; i < code.length; i++)
            {
                buffers.slidingWindow[Wrap((nextChar + i), WINDOW_SIZE)] =
                        buffers.uncodedLookahead[i];
            }
            nextChar = Wrap((nextChar + code.length), WINDOW_SIZE);
        }
    }
    BitFileToFILE(bfpIn);
    return 0;
}
