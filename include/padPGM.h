/**
 * @file padPGM.h
 * @author xyannick3 and CarapaceRose
 * @brief prototype of the public functions of the padPGM.h module.
 */
#ifndef __PAD_PGM_H__
#define __PAD_PGM_H__


/**
 * @brief takes an input file and stores in an output file the file but made compliant to the compression format.
 * 
 * @param input_file name & path of the input file.
 * @param output_file name & path  of the outputfile.
 * @return return 0 in case of success, else -1.
 */
int padPGM(const char* input_file, const char* output_file);

#endif