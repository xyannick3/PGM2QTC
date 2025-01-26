/**
 * @file BitBufferWrite.c
 * @author xyannick3 and CarapaceRose
 * @brief source code of the bitbuffer module, it is used to write bit per bit into a file.
 */

#include "../include/BitBufferWrite.h"
#include <stdio.h>

void initBitBuffer(BitBuffer *bitBuffer){
    bitBuffer->buffer = 0;
    bitBuffer->bitCount = 0;
}

void writeBit(BitBuffer* bitBuffer, unsigned char bit, FILE* file){
    if (bitBuffer->bitCount >= MAX_BITS){
        fwrite(&(bitBuffer->buffer), sizeof(unsigned char), 1, file);
        bitBuffer->buffer = 0;
        bitBuffer->bitCount = 0;
    }

    bitBuffer->buffer |= (bit << (MAX_BITS - 1 - bitBuffer->bitCount));
    bitBuffer->bitCount++;
}


int flushBuffer(BitBuffer* bitBuffer, FILE* file){
    if (bitBuffer->bitCount > 0){
        fwrite(&(bitBuffer->buffer), sizeof(unsigned char), 1, file);
        return MAX_BITS - bitBuffer->bitCount;
    }
    return 0;
}


