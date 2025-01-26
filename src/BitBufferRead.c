/**
 * @file BitBufferRead.c
 * @author xyannick3 and CarapaceRose
 * @brief source code of the bitbuffer module's public functions
 */

#include "../include/BitBufferRead.h"
#include "../include/BitBufferWrite.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * @brief initializes the buffer to read from a text file.
 * @param bitReader the bitReader that's been initialized
 * @param file the file to be read.
 */
void initBitReader(BitReader *bitReader, FILE *file){
    bitReader->buffer = 0;
    bitReader->bitcount = 0;
    bitReader->file = file;
}

/**
 * @brief read a single bit.
 * @param bitReader the bitreader.
 * @param bit the bit output.
 * @return 1 if successful, 0 if EOF.
 */
int readBit(BitReader *bitReader, unsigned char *bit){
    if(bitReader->bitcount ==0){
        if (fread(&(bitReader->buffer), sizeof(unsigned char), 1, bitReader->file) != 1){
            //EOF
            return 0;
        }
        bitReader->bitcount = MAX_BITS;
    }
    *bit = (bitReader->buffer >> (bitReader->bitcount -1)) & 1;
    bitReader->bitcount--;
    return 1;
}
/**
 * @brief read a Byte (8 bits).
 * @param bitReader the bitreader.
 * @param bit the output.
 * @return 1 if successful, 0 if EOF.
 */
int readByte(BitReader *bitReader, unsigned char *byte) {
    *byte = 0; // Initialize the byte to zero

    for (int i = 0; i < MAX_BITS; i++) {
        unsigned char bit;
        if (!readBit(bitReader, &bit)) {
            return 0; // End of file or error
        }
        // Shift the byte left and add the new bit
        *byte = (*byte << 1) | bit;
    }

    return 1; // Successfully read one byte
}
/**
 * @brief read two bits.
 * @param bitReader the bitreader.
 * @param bit the output.
 * @return 1 if successful, 0 if EOF.
 */
int readTwoBits(BitReader *bitReader, unsigned char *value){
    *value = 0;
    for (int i = 0; i < 2 ; i++){
        unsigned char bit;
        if (!readBit(bitReader, &bit)){
            return 0;
        }
        *value = (*value << 1) | bit;
    }
    return 1;
}