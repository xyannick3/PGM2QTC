/**
 * @file main.c
 * @author xyannick3 and CarapaceRose
 * @brief the main file.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../include/QuadTree.h"
#include "../include/padPGM.h"
#include "../include/filtrage.h"
#include "../include/writePGM.h"
#include "../include/writeQTC.h"
#include "../include/readPGM.h"
#include "../include/ReadQTC.h"
#include "../include/SegGrid.h"

/**
 * @brief prints the help display.
 * 
 */
void print_help(){
    printf("Usage:\n");
    printf("-m                multithreading mode (experimental)\n");
    printf(" -c               Encode mode\n");
    printf(" -u               Decode mode\n");
    printf(" -p               converts a non conform PGM file into a compliant one.\n");
    printf(" -i <input_file>  Input file (must be .pgm or qtc depending on the mode)\n");
    printf(" -o <output_file> Output file( default: out.{qtc|pgm})\n");
    printf(" -g               Edit segmentation grid\n");
    printf(" -a <alpha>        Compression factor for lossy encoding (default: 1.0)\n");
    printf(" -h               Display this help message\n");
    printf(" -v               Verbose mode\n");
    exit(0);
}



int main(int argc, char** argv){


    QuadTree * quadtree = NULL;
    unsigned char* pixels = NULL;
    int width = 0, depth = 0;
    double alpha = 1.0;
    int encode = 0, decode = 0, pad = 0, verbose = 0, edit_grid = 0, multithread = 0;
    char *input_file = NULL;
    char *output_file = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "mcupi:o:gvha:")) != -1){
        switch (opt) {
            case 'm':
                multithread = 1;
            case 'c':
                encode = 1;
                break;
            case 'u':
                decode = 1;
                break;
            case 'p':
                pad = 1;
                break;
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'g':
                edit_grid = 1;
                break;
            case 'a':
                alpha = atof(optarg);
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
                print_help();
                break;
            default:
                fprintf(stderr, "Invalid option -%c\n", optopt);
                print_help();
                break;
        }
    }

    if (encode + decode + pad > 1) {
        fprintf(stderr, "Error : Cannot specify multiple modes (-c, -u, -p).\n");
        print_help();
        return 1;
    }
    if (!encode && !decode && !pad) {
        fprintf(stderr, "Error: Must specify a mod (-c, -u, -p).\n");
        print_help();
        return 1;
    }
    if (input_file == NULL){
        fprintf(stderr, "Error: Input file (-i) is required.\n");
        return 1;
    }
    if (output_file == NULL){
        if (encode) {
            output_file = "QTC/out.qtc";
        } else { //works  for both padded and decode.
            output_file = "PGM/out.pgm";
        }
    }
    if (verbose) {
        printf("Mode: %s\n", encode ? "Encode" : decode ? "Decode" : "Pad");
        printf("Input file: %s\n", input_file);
        printf("Output file: %s\n", output_file);
        printf("grid: %s\n", edit_grid ? "Yes" : "No");
    }
    
    if (pad) {
        if (verbose) printf("Padding PGM file %s...\n", input_file);
        if(padPGM(input_file, output_file) == 0) {
            if(verbose) printf("Padded image saved to %s\n", output_file);
        } else {
            fprintf(stderr, "Failed to pad the PGM file.\n");
            return 1;
        }
    }

    if (encode) {
        if (verbose) printf("Encode %s to %s...\n", input_file, output_file);
        pixels = readPGM(input_file, &width);
        depth = (int)log2(width);
        quadtree = initQuadTree(depth);
        if(multithread) {
            init_multithread_filltree(quadtree, pixels, 0, 0, width, 0, width);
        }else {
            fillQuadTree(quadtree, pixels, 0, 0, width, 0, width);
        }
        if (verbose) printf("Quadtree created from PGM file: %s\n",input_file);
        if (!pixels){
            fprintf(stderr, "Failed to read the PGM file. \n");
        }
        //if (verbose) printQuadTree(quadtree);
        if (alpha != 1.0){
            filtrage(quadtree, alpha);
        }
        if (quadtree){
            //writeQuadTreeFileNoLoss(output_file, quadtree, width, depth);
            writeQTCFile(output_file, quadtree, width, depth);
            if (verbose) printf("Quadtree saved to file: %s\n", output_file);
        }else {
            fprintf(stderr, "No quadtree loaded. Use -b or -l option first.\n");
        }
        if(edit_grid){
            if (verbose) printf("Generating the grid\n");
            writeGridFromQuadTree(quadtree, "PGM/grid.pgm", quadtree->width);
            //TODO the grid thing, still have to figure this out.
        }
        if (alpha != 1.0 && verbose){
            printf("Using lossy compression with alpha = %.2f\n", alpha);
        }

    }
    if (decode) {
        if (verbose) printf("Decoding %s to %s...\n", input_file, output_file);
        quadtree = readQTC(input_file);
        if(!quadtree){
            fprintf(stderr ,"No quadtree loaded.");
            return 1;
        }
        if (edit_grid) {
            if (verbose) printf("Generating the grid\n");
            writeGridFromQuadTree(quadtree, "grid.pgm", quadtree->width);
        }
        writePGMFromQuadTree(quadtree, output_file, quadtree->width);
    }
    if(quadtree)
        freeQuadTree(quadtree);

}