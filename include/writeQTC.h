/**
 * @file writeQTC.h
 * @author xyannick3 and CarapaceRose
 * @brief prototype of the public functions and structures of the modude writeQuadTree.
 */
#ifndef __WRITEQUADTREE_H__
#define __WRITEQUADTREE_H__

#include "QuadTree.h"
#include "BitBufferWrite.h"

/**
 * @brief write the quadtree into the qtc file.
 * @param filename the name of the qtc file.
 * @param tree the quadtree.
 * @param imageSize the width of the image.
 * @param depth the depth.
 */
void writeQTCFile(const char* filename, QuadTree* tree, int imageSize, int depth);

#endif