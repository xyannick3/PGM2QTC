/**
 * @file ReadQTC.h
 * @author xyannick3 and CarapaceRose
 * @brief prototype of the public functions and structures of the modude ReadQTC.
 */
#ifndef __READQTC_H__
#define __READQTC_H__
#include "QuadTree.h"
/**
 * @brief readQTC takes in input the name of a qtc file and returns a quadtree.
 * @param filename the name of the qtc file.
 * @return a quadtree.
 */
QuadTree* readQTC(const char* filename);

#endif