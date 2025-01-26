/**
 * @file writePGM.c
 * @author xyannick3 and CarapaceRose
 * @brief source file of the writePGM module.
 */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/writePGM.h"
#include "../include/QuadTree.h"

/**
 * @brief fills the pixmap with the selected node at the said coordinates.
 * @param pixels the pixmap.
 * @param nodes the node array.
 * @param nodeIndex the index of the node.
 * @param x the x coordinate of the pixel.
 * @param y the y coordinate of the pixel.
 * @param size the size of the area.
 * @param width the width of the image.
 */
void fillPixels(unsigned char *pixels, QuadTreeNode* nodes, int nodeIndex, int x, int y, int size, int width) {
    if(nodes[nodeIndex].leaf){
        pixels[y * width + x] = nodes[nodeIndex].m;
        return;
    }
    
    int halfSize = size / 2;

    fillPixels(pixels, nodes, nodeIndex * 4 + 1, x, y, halfSize, width);
    fillPixels(pixels, nodes, nodeIndex * 4 + 2, x + halfSize, y, halfSize, width);
    fillPixels(pixels, nodes, nodeIndex * 4 + 3, x + halfSize, y + halfSize, halfSize, width);
    fillPixels(pixels, nodes, nodeIndex * 4 + 4, x, y + halfSize, halfSize, width);
}
/**
 * @brief write the PGM from the quadtree
 * @param tree the quadtree
 * @param filename the name of the pgm file.
 * @param size the width of the image.
 */
int writePGMFromQuadTree(QuadTree* tree, const char* filename, int size) {
	time_t current_time;
	unsigned char* pixels = malloc(sizeof(unsigned char) * size*size);
    if (pixels == NULL) {
        fprintf(stderr, "Error in the memory allocation process");
        return 0;
    }
    // Call function
    fillPixels(pixels, tree->nodes, 0, 0, 0, size, size);

    // Open file in writing mode
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