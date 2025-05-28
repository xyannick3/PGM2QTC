/**
 * @file filtrage.h
 * @author xyannick3 and CarapaceRose
 * @brief prototype of the public functions of the filtrage module.
 */


#ifndef __FILTRAGE_H__
#define __FILTRAGE_H__
#include "QuadTree.h"
/**
 * @brief function that filter the nodes of the quadtree 
 * @param tree the quadtree that's to be filtered.
 * @param alpha the value that determines how much the quadtree is to be filtered.
 */
void filtrage(QuadTree* tree, double alpha);
#endif