/**
 * @file SegGrid.c
 * @author xyannick3 and CarapaceRose
 * @brief source file of the SegGrid module.
 */
#include "../include/SegGrid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/**
 * @brief fills the pixelmap in the goal of displaying a grid of the segmentation.
 * @param pixels the pixmap.
 * @param nodes the nodes of the quadtree.
 * @param nodeIndex the index of the studied node
 * @param x the x coordinate of the top left of the segment
 * @param y the y coordinate of the top left of the segment.
 * @param size the size of the segment.
 * @param width the width of the image.
 * 
 */
void fillPixelsSegGrid(unsigned char *pixels,QuadTreeNode* nodes, 
int nodeIndex, int x, int y, int size, int width) {
    if (size==1) {
        return;
    }
    if (nodes[nodeIndex].u || nodes[nodeIndex].leaf) {
        for (int i = 0; i < size; i++ ){
            // top & bottom borders
            pixels[(y * width) + (x + i)] = 0;              //top border
            pixels[((y + size - 1) * width) + (x + i)] = 0; //bottom border
            //left and right borders
            pixels[((y + i) * width) + x] = 0;
            pixels[((y + i) * width) + (x + size - 1)] = 0;
        }
        return;
    }

    int halfSize = size / 2;

    fillPixelsSegGrid(pixels, nodes, nodeIndex * 4 + 1, x, y, halfSize, width);
    fillPixelsSegGrid(pixels, nodes, nodeIndex * 4 + 2, x + halfSize, y, halfSize, width);
    fillPixelsSegGrid(pixels, nodes, nodeIndex * 4 + 3, x + halfSize, y + halfSize, halfSize, width);
    fillPixelsSegGrid(pixels, nodes, nodeIndex * 4 + 4, x, y + halfSize, halfSize, width);
}


/**
 * @brief writes the gridPGM file that showcase the grid of the qtc.
 * @param tree the quadtree
 * @param filename the name of the output file.
 * @param size the width of the image.
 */
int writeGridFromQuadTree(QuadTree* tree, const char* filename, int size) {
	time_t current_time;
	unsigned char* pixels = malloc(sizeof(unsigned char) * size*size);
    if (pixels == NULL) {
        fprintf(stderr, "Error in the memory allocation process");
        return 0;
    }
    for(int i = 0; i < size * size; i++) pixels[i] = 255;

    // Call function
    fillPixelsSegGrid(pixels, tree->nodes, 0, 0, 0, size, size);

    // Open file in lecture mode
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0; // Failure
    }
    
    time(&current_time);
	// Writting heading
	fprintf(file, "P5\n");
	fprintf(file, "# Created by FM at : %s", ctime(&current_time));
    fprintf(file,"%d %d\n",tree->width,tree->width);
    fprintf(file,"255\n");


	// Writting	pixels
    fwrite(pixels, sizeof(unsigned char), size * size, file);

    // Close file
    free(pixels);
    if (fclose(file) != 0) {
        perror("Erreur lors de la fermeture du fichier");
        return 0; // Failure
    }

    return 1; // success
}