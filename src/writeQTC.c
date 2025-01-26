/**
 * @file writeQTC.c
 * @author xyannick3 and CarapaceRose
 * @brief source file of the writeQTC module.
 */
#include "../include/writeQTC.h"
#include "../include/BitBufferWrite.h"
#include <time.h>
#include <unistd.h>

/**
 * @brief verifies if the index is the fourth child
 * @param nodeIndex the index
 * @return 1 if it's the fourth child, zero otherwise.
 */
int isFourthChild2(int nodeIndex){
    if (nodeIndex == 0)
        return 0;
    int parentIndex = (nodeIndex -1 ) / 4;
    return nodeIndex == (4 * parentIndex + 4);
}
/**
 * @brief goes through the tree to precalculate the size of the compressed.
 * @param tree the quadtree.
 * @param nodeIndex the index.
 * @param m the number of m that would be written.
 * @param e the number of e that would be written.
 * @param u the number of u that would be written.
 */
void calculateSize(QuadTree *tree, int nodeIndex,long *m, long *e, long *u){
    if(tree->nodes[nodeIndex].parentIndex !=-1){
        if(tree->nodes[tree->nodes[nodeIndex].parentIndex].u){
            return;
    }
    }
    if(nodeIndex == 0 || !isFourthChild2(nodeIndex)) {
        (*m)++;
    }
    if(!tree->nodes[nodeIndex].leaf){
        (*e)++;
        if(!tree->nodes[nodeIndex].epsilon){
            (*u)++;
        }
    }
}
/**
 * @brief recursive function that encodes the quadtree in the output file.
 * @param file the file.
 * @param tree the quadtree.
 * @param nodeIndex the index.
 * @param bitbuffer the bitbuffer.
 */
void encodeQuadTree(FILE* file, QuadTree* tree, int nodeIndex, BitBuffer* bitBuffer){
    if(tree->nodes[nodeIndex].parentIndex !=-1){
        if(tree->nodes[tree->nodes[nodeIndex].parentIndex].u){
            return;
    }
    }
    if(nodeIndex == 0 || !isFourthChild2(nodeIndex)) {
        for (int j = 7; j >= 0; --j) {
            writeBit(bitBuffer, (tree->nodes[nodeIndex].m >> j) & 0x01, file);
        }
    }
    if(!tree->nodes[nodeIndex].leaf){
        for (int i = 1; i >= 0; --i) {
            writeBit(bitBuffer, (tree->nodes[nodeIndex].epsilon >> i) & 0x01, file);
        }
        if(!tree->nodes[nodeIndex].epsilon){
            writeBit(bitBuffer, (tree->nodes[nodeIndex].u), file);

        }
    }
}

/**
 * @brief function that calculates the compression rate.
 * @param originalSize the original Size of the file.
 * @param compressedSize the compressed size.
 * @return the rate of compression.
 */
float calculateCompressionRate2(long originalSize, long compressedSize){
    return (float)((float)compressedSize / (float)originalSize) *100;
}

/**
 * @brief write the quadtree into the qtc file.
 * @param filename the name of the qtc file.
 * @param tree the quadtree.
 * @param imageSize the width of the image.
 * @param depth the depth.
 */
void writeQTCFile(const char* filename, QuadTree* tree, int imageSize, int depth){
    FILE* file = fopen(filename,"wb");
    if(!file){
        fprintf(stderr, "Failed to open file for writing.\n");
        return;
    }
    fprintf(file,"Q1\n");
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char dateStr[30];
    strftime(dateStr, sizeof(dateStr), "# %a %b %d %H:%M:%S %Y",t);
    fprintf(file, "%s\n", dateStr);
    int uncompressedSize = imageSize * imageSize *8;
    Counter counter;
    counter.e=0;
    counter.m=0;
    counter.u=0;
    for(int i = 0; i < tree->totalNodes;  i++){
    calculateSize(tree, i,&counter.m, &counter.e, &counter.u);
    }
    long compressedSize = counter.m*8 + counter.e *2 + counter.u;
    compressedSize += compressedSize % 8 ;
    float compressionRate = calculateCompressionRate2(uncompressedSize, compressedSize);
    
    fprintf(file, "# compression rate %.2f%%\n", compressionRate);
    BitBuffer bitBuffer;
    
    initBitBuffer(&bitBuffer);
    for (int i = 7; i >= 0; --i) {
        writeBit(&bitBuffer, (tree->depth >> i) & 0x01, file);
    }

    for(int i = 0; i < tree->totalNodes;  i++){
        encodeQuadTree(file, tree, i, &bitBuffer);
    }
    
    flushBuffer(&bitBuffer, file);
    
    fclose(file);
}