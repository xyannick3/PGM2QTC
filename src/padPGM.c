/**
 * @file filtrage.c
 * @author xyannick3 and CarapaceRose
 * @brief source file of the padPGM.h module.
 */
#include "../include/padPGM.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int next_power_of_two(int n) {
    int power = 1;
    while (power < n) {
        power*=2;
    }
    return power;
}

unsigned char* readPGM_for_pad(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if(!file) {
        fprintf(stderr, "Unable to open file\n");
        return NULL;
    }
    char format[3];
    fscanf(file, "%s", format);
    if(strcmp(format, "P5") !=0 && strcmp(format, "P2")!=0) {
        fprintf(stderr, "Unsupported PGM format: only P2 and P5 are supported\n");
        fclose(file);
        return NULL;
    }
    char ch = fgetc(file);
    while ((ch=fgetc(file)) == '#') {
        while (fgetc(file) != '\n');
    }
    ungetc(ch, file);
    
    fscanf(file, "%d %d", width, height);
    int maxGray;
    fscanf(file, "%d", &maxGray);

    unsigned char* pixels = (unsigned char*)malloc((*width)*(*height)*sizeof(unsigned char));
    if(!pixels) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }
    if(strcmp(format, "P5")==0) {
        fgetc(file);
        fread(pixels, sizeof(unsigned char), (*width)*(*height), file);
        fclose(file);
        return pixels;
    }
    //in the case of a p2
    int pixelValue;
    for(int i = 0; i < (*width) * (*height); i++) {
        fscanf(file, "%d", &pixelValue);
        pixels[i] = (unsigned char)pixelValue;
    }
    fclose(file);
    return pixels;

}

int writePGM(unsigned char* pixels, const char* output_file, int size) {
    time_t current_time;
    
    FILE * file = fopen(output_file, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }
    time(&current_time);
    fprintf(file, "P5\n");
    fprintf(file, "# Created by FM at : %s", ctime(&current_time));
    fprintf(file, "%d %d\n", size, size);
    fprintf(file, "255\n");

    fwrite(pixels, sizeof(char), size * size, file);
    free(pixels);
    if(fclose(file) != 0) {
        perror("Error in the closing of the file\n");
        return 0;
    }
    return 1;
}


int padPGM(const char* input_file, const char* output_file) {
    int width, height;
    unsigned char* pixels = readPGM_for_pad(input_file, &width, &height);
    if (pixels == NULL) {
        fprintf(stderr, "Failed to read PGM file: %s\n", input_file);
        return -1;
    }
    int new_size = next_power_of_two(width > height ? width : height);

    if (width == height && new_size == width)
        printf("Image is already compliant. nothing to do\n");
    
    unsigned char* padded_pixels = (unsigned char*)calloc(new_size *new_size, sizeof(unsigned char));
    if (!padded_pixels) {
        fprintf(stderr, "Memory allocation failed for padded image.\n");
        free(pixels);
        return -1;
    }
    for (int y = 0; y < height; y++) {
        memcpy(padded_pixels + y* new_size, pixels + y * width, width);
    }
    if (!writePGM(padded_pixels, output_file, new_size)) {
        fprintf(stderr, "Failed to write the padded PGM file: %s\n", output_file);
        free(pixels);
        free(padded_pixels);
        return -1;
    }
    printf("Padded image written to %s\n", output_file);
    return 0;
}