#ifndef __PGM_COMPRESSOR_H__
#define __PGM_COMPRESSOR_H__
#include <stdio.h>
#include <stdlib.h>




// UNDER THIS THE CONTENT OF QUADTREE_H
/**
 * @brief structure that helps with the counting.
 */
typedef struct{
    int m, e, u;
}Counter;

/**
 * @brief structure of the quadtree.
 * @param 
 */
typedef struct{
    unsigned char m;
    unsigned char epsilon;
    unsigned char leaf;
    unsigned char u;
    int parentIndex;
}QuadTreeNode;


typedef struct {
    QuadTreeNode* nodes;
    int depth;
    int width;
    int totalNodes;
} QuadTree;

/**
 * @brief initializes the quadtree
 * 
 * @param depth depth is necessary to allocate the necessary memory.
 * @return QuadTree* 
 */
QuadTree* initQuadTree(int depth);

/**
 * @brief take a pixmap then fills the quadtree as asked.
 * 
 * @param tree tree to fill.
 * @param pixels pixelmap
 * @param x must be initialized at 0
 * @param y must be initialized at 0
 * @param size the width of the image
 * @param nodeIndex must be initialized at 0
 * @param width the width of the image.
 */
void fillQuadTree(QuadTree* tree, unsigned char* pixels, int x, int y, int size, int nodeIndex, int width);


/**
 * @brief displays the tree, node by node, used for debugging.
 * 
 * @param tree the tree to display.
 */
void printQuadTree(QuadTree* tree);


// UNDER THIS THE CONTENT OF BITBUFFER_H

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


// UNDER THIS THE CONTENT OF BITBUFFERREAD_H

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


// UNDER THIS THE CONTENT OF FILTRAGE_H
/**
 * @brief function that filter the nodes of the quadtree 
 * @param tree the quadtree that's to be filtered.
 * @param alpha the value that determines how much the quadtree is to be filtered.
 */
void filtrage(QuadTree* tree, double alpha);

// UNDER THIS THE CONTENT OF PADPGM_H


/**
 * @brief takes an input file and stores in an output file the file but made compliant to the compression format.
 * 
 * @param input_file name & path of the input file.
 * @param output_file name & path  of the outputfile.
 * @return return 0 in case of success, else -1.
 */
int padPGM(const char* input_file, const char* output_file);

// UNDER THIS THE CONTENT OF QUADTREE_H



/**
 * @brief initializes the quadtree
 * 
 * @param depth depth is necessary to allocate the necessary memory.
 * @return QuadTree* 
 */
QuadTree* initQuadTree(int depth);

/**
 * @brief frees the quadtree `tree`, pretty straight forward.
 * 
 * @param tree 
 */
void freeQuadTree(QuadTree *tree);

/**
 * @brief take a pixmap then fills the quadtree as asked.
 * 
 * @param tree tree to fill.
 * @param pixels pixelmap
 * @param x must be initialized at 0
 * @param y must be initialized at 0
 * @param size the width of the image
 * @param nodeIndex must be initialized at 0
 * @param width the width of the image.
 */
void fillQuadTree(QuadTree* tree, unsigned char* pixels, int x, int y, int size, int nodeIndex, int width);


/**
 * @brief displays the tree, node by node, used for debugging.
 * 
 * @param tree the tree to display.
 */
void printQuadTree(QuadTree* tree);


// UNDER THIS THE CONTENT OF READPGM_H

/**
 * @brief read a PGM file and returns a pixmap in the form of an array of unsigned character.
 * @param filename the name of the file.
 * @param width the width of the image.
 * @return pixmap.
 */
unsigned char* readPGM(const char* filename, int* width);


// UNDER THIS THE CONTENT OF READQTC_H

/**
 * @brief readQTC takes in input the name of a qtc file and returns a quadtree.
 * @param filename the name of the qtc file.
 * @return a quadtree.
 */
QuadTree* readQTC(const char* filename);

// UNDER THIS THE CONTENT OF SEGGRID_H

/**
 * @brief writes the gridPGM file that showcase the grid of the qtc.
 * @param tree the quadtree
 * @param filename the name of the output file.
 * @param size the width of the image.
 */
int writeGridFromQuadTree(QuadTree* tree, const char* filename, int size);

// UNDER THIS THE CONTENT OF WRITEPGM_H

/**
 * @brief write the PGM from the quadtree
 * @param tree the quadtree
 * @param filename the name of the pgm file.
 * @param size the width of the image.
 */
int writePGMFromQuadTree(QuadTree* tree, const char* filename, int size);

// UNDER THIS THE CONTENT OF WRITEQUADTREE_H
/**
 * @brief write the quadtree into the qtc file.
 * @param filename the name of the qtc file.
 * @param tree the quadtree.
 * @param imageSize the width of the image.
 * @param depth the depth.
 */
void writeQTCFile(const char* filename, QuadTree* tree, int imageSize, int depth);


#endif