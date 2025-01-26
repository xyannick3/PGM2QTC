/**
 * @file ReadQTC.c
 * @author xyannick3 and CarapaceRose
 * @brief source file of the ReadQTC module.
 */
#include "../include/ReadQTC.h"
#include "../include/BitBufferRead.h"
#include "../include/QuadTree.h"
#include <stdio.h>
#include <string.h>

void fillQuadTreefromCompress(FILE *file, QuadTree *quadtree);

/**
 * @brief a little function to determine the depth of a node by its index.
 * @param nodeIndex the index of a node.
 * @return the depth.
 */
int getNodeDepth(int nodeIndex){
    int level = 0;
    int nodesInLevel = 1;
    while (nodeIndex >= nodesInLevel){
        nodeIndex -= nodesInLevel;
        level++;
        nodesInLevel *= 4;
    }
    return level;
}
/**
 * @brief verifies if the node is the fourth child of a tree.
 * @param nodeIndex the node of the index studied
 * @return 1 if it is, else 0.
 */
int isFourthChild(int nodeIndex){
    if (nodeIndex == 0)
        return 0;
    int parentIndex = (nodeIndex -1 ) / 4;
    return nodeIndex == (4 * parentIndex + 4);
}
/**
 * @brief readQTC takes in input the name of a qtc file and returns a quadtree.
 * @param filename the name of the qtc file.
 * @return a quadtree.
 */
QuadTree* readQTC(const char* filename){
    FILE* file = fopen(filename, "rb");
    if(!file){
        fprintf(stderr, "Unable to open file%s\n", filename);
        return NULL;
    }
    char format[3];
    fscanf(file, "%s", format);
    if(strcmp(format, "Q1") != 0 ){
        fprintf(stderr, "Unsupported QTC format, only Q1 is supported\n");
        fclose(file);
        return NULL;
    }
    char ch = fgetc(file);
    while ((ch = fgetc(file)) == '#'){
        while (fgetc(file) != '\n');
    }
    ungetc(ch, file);
    unsigned char depth;
    fread(&depth, sizeof(unsigned char), 1, file);
    printf("depth of %d\n", depth);
    QuadTree *quadtree = initQuadTree(depth);
    fillQuadTreefromCompress(file, quadtree);
    return quadtree;
}
/**
 * @brief fills the node i of the quadtree in the case the parent is unitary.
 * @param quadtree the quadtree
 * @param i the index of the node.
 */
void handleUnitaryParent(QuadTree* quadtree, int i){
    int parentIndex = quadtree->nodes[i].parentIndex;
    quadtree->nodes[i].epsilon = 0;
    quadtree->nodes[i].u = 1;
    quadtree->nodes[i].m = quadtree->nodes[parentIndex].m;
    quadtree->nodes[i].leaf = (getNodeDepth(i) == quadtree->depth) ? 1 : 0;
}
/**
 * @brief fills the quadtree in the case the node is the fourth of a branch.
 * @param quadtree the quadtree.
 * @param i the index
 * @param bitreader the bitreader
 */
void handleFourthChild(QuadTree* quadtree, int i, BitReader* bitreader){
    int parentIndex = quadtree->nodes[i].parentIndex;
    int siblingStartIndex = 4 * parentIndex +1;
    int sumOfSiblings = 0;
    for (int j = siblingStartIndex; j < siblingStartIndex + 3;j++){
        sumOfSiblings += quadtree->nodes[j].m;
    }

    int sumOfChildren = 4 * quadtree->nodes[quadtree->nodes[i].parentIndex].m + quadtree->nodes[quadtree->nodes[i].parentIndex].epsilon;
    quadtree->nodes[i].m = sumOfChildren - sumOfSiblings;
    quadtree->nodes[i].leaf = 0;
    if(getNodeDepth(i) == quadtree->depth){
        quadtree->nodes[i].leaf = 1;
        quadtree->nodes[i].epsilon=0;
        quadtree->nodes[i].u = 0;
    }else{
        readTwoBits(bitreader, &quadtree->nodes[i].epsilon);
        if(quadtree->nodes[i].epsilon == 0){
            readBit(bitreader, &quadtree->nodes[i].u);
        }else{
            quadtree->nodes[i].u=0;
        }
    }
}
/**
 * @brief fills the node of a quadtree.
 * @param quadtree the quadtree
 * @param i the index
 * @param bitreader the bitreader.
 */
void theNormalWayyyy(QuadTree * quadtree, int i, BitReader *bitReader){
    readByte(bitReader, &quadtree->nodes[i].m);
    quadtree->nodes[i].leaf = 0;
    if(getNodeDepth(i) == quadtree->depth){
        quadtree->nodes[i].leaf = 1;
        quadtree->nodes[i].epsilon=0;
        quadtree->nodes[i].u = 0;
    }else{
        readTwoBits(bitReader, &quadtree->nodes[i].epsilon);
        if(quadtree->nodes[i].epsilon == 0){
            readBit(bitReader, &quadtree->nodes[i].u);
        }else{
            quadtree->nodes[i].u = 0;
        }
    }
}
/**
 * @brief fills the quadtree from the file
 * @param file the file 
 * @param quadtree the quadtree.
 */
void fillQuadTreefromCompress(FILE *file, QuadTree *quadtree){
    
    int i = 0;
    //we read the first node
    BitReader bitReader;
    initBitReader(&bitReader, file);
    readByte(&bitReader, &quadtree->nodes[i].m);
    readTwoBits(&bitReader, &quadtree->nodes[i].epsilon);
    if(quadtree->nodes[i].epsilon == 0){
        readBit(&bitReader, &quadtree->nodes[i].u);
    }else{
        quadtree->nodes[i].u=0;
    }
    i++;
    //now we enter the complicated part
    for(;i<quadtree->totalNodes;i++){
        quadtree->nodes[i].parentIndex = (i-1) / 4;
        //if the parent node is unitary then the children inherit that trait.
        if(quadtree->nodes[quadtree->nodes[i].parentIndex].u){
            handleUnitaryParent(quadtree, i);

        }else{
            //deals with the case in which the child is the 4th of a parent 
            if(isFourthChild(i)){
                handleFourthChild(quadtree, i, &bitReader);
            }else{
                theNormalWayyyy(quadtree, i, &bitReader);
            }
        }
    }
}