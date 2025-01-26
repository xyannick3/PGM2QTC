/**
 * @file BitBufferRead.h
 * @author xyannick3 and CarapaceRose
 * @brief source code of the bitbuffer module, it is used to write bit per bit into a file.
 */


#ifndef __BITBUFFERREADER_H__
#define __BITBUFFERREADER_H__ 
#define MAX_BITS 8
#include <stdio.h>

/**
 * @brief the structure of the BitReader
 * 
 */
typedef struct{
    unsigned char buffer;
    int bitcount;
    FILE *file;
} BitReader;
/**
 * @brief initializes the buffer to read from a text file.
 * @param bitReader the bitReader that's been initialized
 * @param file the file to be read.
 */
void initBitReader(BitReader *bitReader, FILE *file);
/**
 * @brief read a single bit.
 * @param bitReader the bitreader.
 * @param bit the bit output.
 * @return 1 if successful, 0 if EOF.
 */
int readBit(BitReader *bitReader, unsigned char *bit);
/**
 * @brief read a Byte (8 bits).
 * @param bitReader the bitreader.
 * @param bit the output.
 * @return 1 if successful, 0 if EOF.
 */
int readByte(BitReader *bitReader, unsigned char *byte);
/**
 * @brief read two bits.
 * @param bitReader the bitreader.
 * @param bit the output.
 * @return 1 if successful, 0 if EOF.
 */
int readTwoBits(BitReader *bitReader, unsigned char *value);
#endif 