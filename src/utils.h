#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>

// Input files management
void readDouble(FILE * file, char * name,  double * value);
void readInt(FILE * file, char * name,  int * value);
void writeHeader(FILE * fileOut, char * header[]);
void printInfo(char * info);

// Maths functions


#endif
