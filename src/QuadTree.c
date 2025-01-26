/**
 * @file QuadTree.c
 * @author xyannick3 and CarapaceRose
 * @brief source file of the QuadTree module.
 */

#include "../include/QuadTree.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
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