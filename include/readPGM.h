/**
 * @file readPGM.h
 * @author xyannick3 and CarapaceRose
 * @brief prototype of the public functions and structures of the modude readPGM.
 */
#ifndef __PGM_H__
#define __PGM_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief read a PGM file and returns a pixmap in the form of an array of unsigned character.
 * @param filename the name of the file.
 * @param width the width of the image.
 * @return pixmap.
 */
unsigned char* readPGM(const char* filename, int* width);


#endif