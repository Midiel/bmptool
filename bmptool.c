/*
 *
 * This program manipulates an image from a 24-bit uncompressed bmp file.
 * The user options are obtained from the command line using getopt. The image
 * can be scaled by a positive integer, rotated by a multiple of 90 (clockwise 
 * and counter-clockwise), flipped horizontally, and flipped vertically.
 * If no input file is provided, then the file is obtained from standard input.
 * If no output file is provided, then the file is sent to standard output.
 * 
 *
 * Disclaimer:
 * I hereby certify that this code is my work and my work alone and understand
 * the syllabus regarding plagiarized code.
 * Copyright 2018 Midiel
 */

/*
 * File:   bmptool.c
 *
 * Created on July 14, 2018
 */

#include <stdlib.h>
#include "bmplib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>



// It displays/prints the proper usage to run the program.
static void print_usage() {
    printf("Usage: bmptool [-s scale] [-r degree] [-f] [-v ] [-o output_file] [input_file]\n");
}

// it  displays memory error message
static void momoryError() {
    printf("Error. Allocation of memory was unsuccessful.\n");
}

// double entry error
static void print_double_entry(char* a) {
    printf("Error: %s can only be entered once.\n", a);
}


/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new1      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL* original, int rows, int cols, int scale,
	    PIXEL** new, int* newrows, int* newcols)
{
  
  /* THIS IS THE METHOD THAT YOU SHOULD WRITE */
    int row, col;
    if((rows<=0) || (cols<=0)) {
        return -1;
    }

    (*newrows) = (int)(rows * scale);
    (*newcols) = (int)(cols * scale);

    *new = (PIXEL*)malloc((*newrows) * (*newcols) * sizeof(PIXEL));

    for(row=0; row < (*newrows); row++) {
        for(col=0; col < (*newcols); col++) {
            PIXEL* o = ((original) + (row * rows / (*newrows)) * cols + col * cols / (*newcols));
            PIXEL* n = (*new) + (row * (*newcols) + col);
            *n = *o;
        }
    }
    
    return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation,
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation,
	   PIXEL** new, int* newrows, int* newcols)
{
    /* THIS IS THE METHOD THAT YOU SHOULD WRITE */
    
    // variables
    int col, row;
    
    // allocates memory for the new bitmap
    *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));
        
    if(rotation == 90 || rotation == -270) {            // rotates 90 or -270
        *newrows = cols;
        *newcols = rows;
        for(row = 0; row < rows; row++) {
            for(col = 0; col < cols; col++ ) {
                PIXEL* o = ((original) + (row * cols) + col);
                PIXEL* n = (*new) + (cols-col-1) * rows + row;       
                *n = *o;
            }
        }    
    } else if(rotation == 180 || rotation == -180) {    //rotates 180 or -180
        *newrows = rows;
        *newcols = cols;
        for(row = 0; row < rows; row++) {
            for(col = 0; col < cols; col++ ) {   
                PIXEL* o = ((original) + (row * cols) + col);
                PIXEL* n = (*new) + (rows-row)*cols - (col+1);
                *n = *o;
            }
        } 
    } else if(rotation == 270 || rotation == -90) {        //rotates 270 or -90
        *newrows = cols;
        *newcols = rows;
        for(row = 0; row < rows; row++) {
            for(col = 0; col < cols; col++ ) {
                PIXEL* o = ((original) + (row * cols) + col);
                PIXEL* n = (*new) + (col * rows) + (rows - row - 1 );
                *n = *o;
            }
        }
    } 
 
    return 0;
}

/*
 * This method Vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int verticalflip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  /* THIS IS THE METHOD THAT YOU SHOULD WRITE */
    int row, col;

    if ((rows <= 0) || (cols <= 0)) {
        return -1;
    }

    *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

    for (row=0; row < rows; row++) {
        for (col=0; col < cols; col++) {
            PIXEL* o = original + row*cols + col;
            PIXEL* n = (*new) + (rows - row - 1) * cols + col;
            *n = *o;
        }
    }
    return 0;
}


/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  for (row=0; row < rows; row++)
    for (col=0; col < cols; col++) {
      PIXEL* o = original + row*cols + col;
      PIXEL* n = (*new) + row*cols + (cols-1-col);
      *n = *o;
    }

  return 0;
}

/*
 * This method hcopies a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int copyBitmap(PIXEL *original, PIXEL **new, int rows, int cols) {
    
    // variables
    int row, col;
    free(*new);
    if ((rows <= 0) || (cols <= 0)) return -1;
    
    *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

    for (row=0; row < rows; row++) {
        for (col=0; col < cols; col++) {
            PIXEL* o = original + row*cols + col;
            PIXEL* n = (*new) + row*cols + col;
            *n = *o;
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    
    // To hold the flags and variables
    extern char *optarg;
    extern int optind;
    
    int r, c, nr, nc;
    PIXEL *b;
    PIXEL *newImage;
    char* outFile;

    int sFlag = 0;
    int rFlag = 0;
    int fFlag = 0;
    int vFlag = 0;
    int oFlag = 0;
    int scale = 0;
    int degree = 0;
    
    // getopt loop to options options from command
    int a;
    while ((a = getopt(argc, argv, "s:r:fvo:")) != -1){
        switch (a) {
            case 's':                                   // sets sFlag
                if(sFlag) {                             // scale maximum one time
                    print_double_entry("Scale");
                    print_usage();
                    return -1;
                }
                
                scale = atoi(optarg);                   // gets the scale
                if(scale < 1) {                         // validates scale is positive
                    printf("Error: Scale must be a positive integer greater than 0.\n");
                    print_usage();
                    return -1;
                }
                
                sFlag = 1;                              // sets the flag
                break;        
            case 'r':                                   // sets rFlag
                if(rFlag) {                             // rotations maximum one time
                    print_double_entry("Rotation");
                    print_usage();
                    return -1;
                }
                
                degree = atoi(optarg);                  // gets rotation value
                if((degree % 90) != 0) {                // validates rotation multiple of 90
                    printf("Error: Degree must be a multiple of 90.\n");
                    print_usage();
                    return -1;
                }
                
                degree = degree % 360;                  // divides degree in quarters
                rFlag = 1;
                break;
            case 'f':                                   // sets fFlag
                if(fFlag) {                             // flip option maximum one time
                    print_double_entry("Flip");
                    print_usage();
                    return -1;
                }
                fFlag = 1;
                break;
            case 'v':                                   // sets vFlag
                if(vFlag) {                             // vertical flip maximum one time
                    print_double_entry("Vertical Flip");
                    print_usage();
                    return -1;
                }
                vFlag = 1;
                break;
            case 'o':                                   // sets oFlag
                if(oFlag) {                             // output file only one time
                    print_double_entry("Output File");
                    print_usage();
                    free(outFile);
                    return -1;
                }
                
                outFile = (char*)malloc(strlen(optarg) * sizeof(char));
                
                // Validate allocation of memory
                if(outFile == NULL) {
                    momoryError();
                    return -1;
                }
                outFile = optarg;
                oFlag = 1;
                break;
            case '?':
                printf("Error: Invalid format.\n");
                print_usage();
                exit (0);                    
        }
    }
    
    // reads the bitmap file
    if((argv[optind])) {        // if input file was provided, read file
        
        // validate the file reading was successful
        if((readFile(argv[optind], &r, &c, &b)) != 0) {
            return -1;
        }
    } else {                    // if no input file, then read from standard input
        readFile(NULL, &r, &c, &b);    
    }
    
    // enlarge
    if(sFlag) {
        enlarge(b, r, c, scale, &newImage, &nr, &nc);   // enlarge the original image
        copyBitmap(newImage, &b, nr, nc);           // copy new image to original variable
        free(newImage);                             // free the pointer
        r = nr;                                     // copy the number of rows
        c = nc;                                     // copy the number of columns
    }

    // rotate
    if(rFlag) {
        if(degree != 0) {
            rotate(b, r, c, degree, &newImage, &nr, &nc);   // rotates the image
            copyBitmap(newImage, &b, nr, nc);           // copy new image to original variable
            free(newImage);                             // free the pointer
            r = nr;                                     // copy the number of rows
            c = nc;                                     // copy the number of columns
        }
    }
    
    // vertical flip
    if(vFlag) {
        verticalflip(b, &newImage, r, c);               // vertical flips an image
        copyBitmap(newImage, &b, r, c);                 // copy new image to original variable
        free(newImage);                                 // free the pointer
    }
     
    // flips horizontal
    if(fFlag) {
        flip(b, &newImage, r, c);                       // flips an image
        copyBitmap(newImage, &b, r, c);                 // copy new image to original variable 
        free(newImage);                                 // free the pointer
    }
    
    // writes the bitmap image
    if(oFlag) {                                 // if output file, then write to it
        writeFile(outFile, r, c, b);
    } else {                                    // else write to standard output
        writeFile(NULL, r, c, b);
    }
    
    free(b);                                    // free the bitmap pointer

    return 0;
}

