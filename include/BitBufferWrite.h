/**
 * @file BitBufferWrite.h
 * @author xyannick3 and CarapaceRose
 * @brief source code of the bitBufferWrite module's public functions, it is used to write bit per bit into a file.
 */

#ifndef __BITBUFFERWRITE_H__
#define __BITBUFFERWRITE_H__
#include <stdio.h>
#define MAX_BITS 8

/**
 * @brief buffer structure
 * 
 */
typedef struct {
    unsigned char buffer;
    int bitCount;
} BitBuffer;
/**
 * @brief initializes the buffer.
 * 
 * @param bitBuffer a buffer for bits.
 */
void initBitBuffer(BitBuffer *bitBuffer);

/**
 * @brief writes a bit into the buffer, if the buffer is full transfers it to the file.
 * 
 * @param bitBuffer the buffer.
 * @param bit the bit.
 * @param file the output file.
 */
void writeBit(BitBuffer* bitBuffer, unsigned char bit, FILE* file);

/**
 * @brief 
 * 
 * @param bitBuffer flushes the remaining bits from the buffer into the file
 * @param file 
 * @return the amount of bits that have been added to the file.
 */
int flushBuffer(BitBuffer* bitBuffer, FILE* file);



#endif