/**
 * @file SegGrid.h
 * @author xyannick3 and CarapaceRose
 * @brief prototype of the public functions and structures of the modude SegGrid.
 */
#ifndef __SEGGRID_H__
#define __SEGGRID_H__
#include "QuadTree.h"



/**
 * @brief writes the gridPGM file that showcase the grid of the qtc.
 * @param tree the quadtree
 * @param filename the name of the output file.
 * @param size the width of the image.
 */
int writeGridFromQuadTree(QuadTree* tree, const char* filename, int size);

#endif