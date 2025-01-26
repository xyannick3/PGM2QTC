/**
 * @file writePGM.h
 * @author xyannick3 and CarapaceRose
 * @brief prototype of the public functions and structures of the modude writePGM.
 */
#ifndef __WRITEPGM_H__
#define __WRITEPGM_H__
#include "QuadTree.h"

/**
 * @brief write the PGM from the quadtree
 * @param tree the quadtree
 * @param filename the name of the pgm file.
 * @param size the width of the image.
 */
int writePGMFromQuadTree(QuadTree* tree, const char* filename, int size);

#endif