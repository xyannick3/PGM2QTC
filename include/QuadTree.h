/**
 * @file QuadTree.h
 * @author xyannick3 and CarapaceRose
 * @brief prototype of the public functions and structures of the modude Quadtree.
 */

#ifndef __QUADTREE_H__
#define __QUADTREE_H__
/**
 * @brief structure that helps with the counting.
 */
typedef struct{
    long m, e, u;
}Counter;

/**
 * @brief structure of the quadtree.
 * @param 
 */
typedef struct{
    unsigned char m;
    unsigned char epsilon;
    int leaf;
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

#endif