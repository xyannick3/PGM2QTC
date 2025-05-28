/**
 * @file QuadTree.c
 * @author xyannick3 and CarapaceRose
 * @brief source file of the QuadTree module.
 */

#include "../include/QuadTree.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <pthread.h>
/**
 * @brief structure pour tenir les arguments du thread
 * 
 */
typedef struct {
    QuadTree* tree;
    unsigned char* pixels;
    int x;
    int y;
    int size;
    int nodeIndex; 
    int width;
} arguments;

/**
 * @brief initializes the quadtree
 * 
 * @param depth depth is necessary to allocate the necessary memory.
 * @return QuadTree* 
 */
QuadTree* initQuadTree(int depth){
    QuadTree* tree = (QuadTree*)malloc(sizeof(QuadTree));
    tree->depth = depth;
    tree->totalNodes = (int)((pow(4, depth + 1)-1)/3);
    tree->nodes = (QuadTreeNode*)malloc(tree->totalNodes*sizeof(QuadTreeNode));
    tree->width = pow(2,depth); 
    return tree;
}
/**
 * @brief frees the quadtree `tree`, pretty straight forward.
 * 
 * @param tree 
 */
void freeQuadTree(QuadTree *tree){
    free(tree->nodes);
    free(tree);
}
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
void fillQuadTree(QuadTree* tree, unsigned char* pixels, int x, int y, int size, int nodeIndex, int width) {
    if (size == 1){ //end of recursion condition.
        tree->nodes[nodeIndex].m = pixels[y *width  + x];
        tree->nodes[nodeIndex].epsilon = 0;// No error cause it's a leaf :)
        tree->nodes[nodeIndex].u = 1;
        tree->nodes[nodeIndex].leaf = 1;
        tree->nodes[nodeIndex].parentIndex = (nodeIndex -1)/ 4;
        return;
    }

    int halfSize = size / 2;


    fillQuadTree(tree, pixels, x, y, halfSize, nodeIndex * 4 + 1,width); // Top-left
    fillQuadTree(tree, pixels, x + halfSize, y, halfSize, nodeIndex * 4 + 2,width); // Top-right
    fillQuadTree(tree, pixels, x + halfSize, y + halfSize, halfSize, nodeIndex * 4 + 3,width); // Bottom-right
    fillQuadTree(tree, pixels, x, y + halfSize, halfSize, nodeIndex * 4 + 4,width); // Bottom-left

    unsigned char m1 = tree->nodes[nodeIndex * 4 + 1].m;
    unsigned char m2 = tree->nodes[nodeIndex * 4 + 2].m;
    unsigned char m3 = tree->nodes[nodeIndex * 4 + 3].m;
    unsigned char m4 = tree->nodes[nodeIndex * 4 + 4].m;


    int sum = m1 + m2 + m3 + m4;
    tree->nodes[nodeIndex].m = sum / 4; // Average intensity
    tree->nodes[nodeIndex].epsilon = sum % 4; // Error
    tree->nodes[nodeIndex].u = ((m1 == m2 && m2 == m3 && m3 == m4) &&
    tree->nodes[nodeIndex * 4 + 1].u && tree->nodes[nodeIndex * 4 + 2].u && tree->nodes[nodeIndex * 4 + 3].u && tree->nodes[nodeIndex * 4 + 4].u) ? 1 : 0; // Check uniformity
    tree->nodes[nodeIndex].leaf = 0;
    if(nodeIndex!=0){
    tree->nodes[nodeIndex].parentIndex = (nodeIndex-1)/4;
    }else{
        tree->nodes[nodeIndex].parentIndex = -1;
    }

}
/**
 * @brief displays the tree, node by node, used for debugging.
 * 
 * @param tree the tree to display.
 */
void printQuadTree(QuadTree* tree) {
    for (int i = 0; i < tree->totalNodes; i++) {
        printf("Node %d - m: %d, epsilon: %d, u: %d, l: %d, parent : %d\n", i, tree->nodes[i].m, tree->nodes[i].epsilon, tree->nodes[i].u, tree->nodes[i].leaf, tree->nodes[i].parentIndex);
    }
}


void * mult_fillQuadTree(void* args0) {
    //QuadTree* tree, unsigned char* pixels, int x, int y, int size, int nodeIndex, int width
    arguments *args = (arguments*)args0;
    QuadTree* tree = args->tree;
    unsigned char* pixels = args->pixels;
    int x = args->x;
    int y = args->y;
    int size = args->size;
    int nodeIndex = args->nodeIndex;
    int width = args->width;
    if (size == 1){ //end of recursion condition.
        tree->nodes[nodeIndex].m = pixels[y *width  + x];
        tree->nodes[nodeIndex].epsilon = 0;// No error cause it's a leaf :)
        tree->nodes[nodeIndex].u = 1;
        tree->nodes[nodeIndex].leaf = 1;
        tree->nodes[nodeIndex].parentIndex = (nodeIndex -1)/ 4;
        return NULL;
    }

    int halfSize = size / 2;


    fillQuadTree(tree, pixels, x, y, halfSize, nodeIndex * 4 + 1,width); // Top-left
    fillQuadTree(tree, pixels, x + halfSize, y, halfSize, nodeIndex * 4 + 2,width); // Top-right
    fillQuadTree(tree, pixels, x + halfSize, y + halfSize, halfSize, nodeIndex * 4 + 3,width); // Bottom-right
    fillQuadTree(tree, pixels, x, y + halfSize, halfSize, nodeIndex * 4 + 4,width); // Bottom-left

    unsigned char m1 = tree->nodes[nodeIndex * 4 + 1].m;
    unsigned char m2 = tree->nodes[nodeIndex * 4 + 2].m;
    unsigned char m3 = tree->nodes[nodeIndex * 4 + 3].m;
    unsigned char m4 = tree->nodes[nodeIndex * 4 + 4].m;


    int sum = m1 + m2 + m3 + m4;
    tree->nodes[nodeIndex].m = sum / 4; // Average intensity
    tree->nodes[nodeIndex].epsilon = sum % 4; // Error
    tree->nodes[nodeIndex].u = ((m1 == m2 && m2 == m3 && m3 == m4) &&
    tree->nodes[nodeIndex * 4 + 1].u && tree->nodes[nodeIndex * 4 + 2].u && tree->nodes[nodeIndex * 4 + 3].u && tree->nodes[nodeIndex * 4 + 4].u) ? 1 : 0; // Check uniformity
    tree->nodes[nodeIndex].leaf = 0;
    if(nodeIndex!=0){
    tree->nodes[nodeIndex].parentIndex = (nodeIndex-1)/4;
    }else{
        tree->nodes[nodeIndex].parentIndex = -1;
    }
    return NULL;
}

void init_multithread_filltree(QuadTree* tree, unsigned char* pixels, int x, int y, int size, int nodeIndex, int width) {
    if (size == 1){ //end of recursion condition.
        tree->nodes[nodeIndex].m = pixels[y *width  + x];
        tree->nodes[nodeIndex].epsilon = 0;// No error cause it's a leaf :)
        tree->nodes[nodeIndex].u = 1;
        tree->nodes[nodeIndex].leaf = 1;
        tree->nodes[nodeIndex].parentIndex = (nodeIndex -1)/ 4;
        return;
    }

    int halfSize = size / 2;
    // preparing arguments for NW node
    pthread_t threadNW;
    arguments argsNW;
    argsNW.tree = tree;
    argsNW.pixels = pixels;
    argsNW.x = x;
    argsNW.y = y;
    argsNW.size = halfSize;
    argsNW.nodeIndex =  nodeIndex * 4 + 1;
    argsNW.width = width;
    //preparing arguments for NE node
    pthread_t threadNE;
    arguments argsNE;
    argsNE.tree = tree;
    argsNE.pixels = pixels;
    argsNE.x = x + halfSize;
    argsNE.y = y;
    argsNE.size = halfSize;
    argsNE.nodeIndex = nodeIndex * 4 + 2;
    argsNE.width = width;

    //preparing arguments for SW node
    pthread_t threadSW;
    arguments argsSW;
    argsSW.tree = tree;
    argsSW.pixels = pixels;
    argsSW.x = x;
    argsSW.y = y + halfSize;
    argsSW.size = halfSize;
    argsSW.nodeIndex = nodeIndex * 4 + 4;
    argsSW.width = width;


    //preparing arguments for SE node
    pthread_t threadSE;
    arguments argsSE;
    argsSE.tree = tree;
    argsSE.pixels = pixels;
    argsSE.x = x + halfSize;
    argsSE.y = y + halfSize;
    argsSE.size = halfSize;
    argsSE.nodeIndex = nodeIndex * 4 + 3;
    argsSE.width = width;


    pthread_create(&threadNW, NULL, mult_fillQuadTree, &argsNW);
    pthread_create(&threadNE, NULL, mult_fillQuadTree, &argsNE);
    pthread_create(&threadSE, NULL, mult_fillQuadTree, &argsSE);
    pthread_create(&threadSW, NULL, mult_fillQuadTree, &argsSW);
    
    pthread_join(threadNW, NULL);
    pthread_join(threadNE, NULL);
    pthread_join(threadSE, NULL);
    pthread_join(threadSW, NULL);


    unsigned char m1 = tree->nodes[nodeIndex * 4 + 1].m;
    unsigned char m2 = tree->nodes[nodeIndex * 4 + 2].m;
    unsigned char m3 = tree->nodes[nodeIndex * 4 + 3].m;
    unsigned char m4 = tree->nodes[nodeIndex * 4 + 4].m;


    int sum = m1 + m2 + m3 + m4;
    tree->nodes[nodeIndex].m = sum / 4; // Average intensity
    tree->nodes[nodeIndex].epsilon = sum % 4; // Error
    tree->nodes[nodeIndex].u = ((m1 == m2 && m2 == m3 && m3 == m4) &&
    tree->nodes[nodeIndex * 4 + 1].u && tree->nodes[nodeIndex * 4 + 2].u && tree->nodes[nodeIndex * 4 + 3].u && tree->nodes[nodeIndex * 4 + 4].u) ? 1 : 0; // Check uniformity
    tree->nodes[nodeIndex].leaf = 0;
    if(nodeIndex!=0){
    tree->nodes[nodeIndex].parentIndex = (nodeIndex-1)/4;
    }else{
        tree->nodes[nodeIndex].parentIndex = -1;
    }

}
