/**
 * @file readPGM.c
 * @author xyannick3 and CarapaceRose
 * @brief source file of the readPGM module.
 */
#include "../include/readPGM.h"


/**
 * @brief read a PGM file and returns a pixmap in the form of an array of unsigned character.
 * @param filename the name of the file.
 * @param width the width of the image.
 * @return pixmap.
 */
unsigned char* readPGM(const char* filename, int* width) {
    FILE* file = fopen(filename, "rb");
    if(!file){
        fprintf(stderr,"Unable to open file\n");
        return NULL;
    }
    char format[3];
    fscanf(file,"%s",format);

    if(strcmp(format, "P5") !=0 && strcmp(format, "P2")!=0){
        fprintf(stderr, "Unsupported PGM format: only P2 and P5 are supported format\n");
        fclose(file);
        return NULL;
    }
    char ch = fgetc(file);
    while ((ch =fgetc(file)) == '#'){
        while (fgetc(file) !='\n');
    }
    ungetc(ch, file);


    //read width and height from file
    int width2;
    width2 = 0;
    fscanf(file, "%d %d", width, &width2);
    if(*width !=  width2) {
        perror("incompatible image format\n");
        exit(1);
    }
    int maxGray;
    fscanf(file, "%d", &maxGray);

    //verifies if the dimension is valid
    int n = (int)log2(*width);
    if (*width != (1 << n)){
        fprintf(stderr, "Image dimensions are not a power of 2\n");
        fclose(file);
        exit(1);
    }
    //allocate memory for the pixels
    unsigned char* pixels = (unsigned char*)malloc((*width) * (*width) * sizeof(unsigned char));
    if(!pixels){
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }
    if(strcmp(format, "P5")==0){
        //Format p5 binary so we just transfer each byte
        fgetc(file);
        fread(pixels, sizeof(unsigned char), (*width)*(*width),file);
        fclose(file);
        return pixels;
    }
    //in the case of a p2 binary we 
    int pixelValue;
    for(int i = 0; i < (*width) * *(width); i++){
        fscanf(file, "%d", &pixelValue);
        pixels[i] = (unsigned char)pixelValue;
    }
    fclose(file);
    return pixels;
}
