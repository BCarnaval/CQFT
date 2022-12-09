#include <math.h>
#include <complex.h>
#include <sys/stat.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#define COLOR_RED   "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_CYAN  "\x1b[36m"

void readDouble(FILE *file, char *name,  double *value) {

    rewind(file);

    char tempbuff[200];
    while (!feof(file))
    {
        if (fgets(tempbuff, 200, file))
        {
            char tmpstr1[50];
            char tmpstr2[50];
            sscanf(tempbuff, "%49s %49s\n", tmpstr1, tmpstr2);

            if (strcmp(tmpstr1, name) == 0)
            {
                *value = atof(tmpstr2);
                return;
            }
        }
    }
    printf("\nCannot find the %s parameter in 'model.dat'", name);
    exit(1);
}

void readInt(FILE *file, char *name,  int *value) {

    rewind(file);

    char tempbuff[200];
    while (!feof(file))
    {
        if (fgets(tempbuff, 200, file))
        {
            char tmpstr1[50];
            char tmpstr2[50];
            sscanf(tempbuff, "%49s %49s\n", tmpstr1, tmpstr2);

            if (strcmp(tmpstr1, name) == 0)
            {
                *value = atoi(tmpstr2);
                return;
            }
        }
    }
    printf("\nCannot find the %s parameter in 'model.dat'", name);
    exit(1);
}

void readFile(FILE *fileIn, const int colmn) {

}

void writeHeader(FILE *fileOut, const char *header[], int size) {

    for (int i = 0; i < size; i++)
    {
        if (i == size - 1)
            fprintf(fileOut, "%s", header[i]);

        else
            fprintf(fileOut, "%s         ", header[i]);
    }
    fprintf(fileOut, "\n");
    return;
}

void LOG(const char *info, int color) {
    if (color == 0)
        printf(COLOR_RED   "\n%s\n" COLOR_RESET, info);

    else if (color == 1)
        printf(COLOR_GREEN "\n%s\n" COLOR_RESET, info);

    else if (color == 2)
        printf(COLOR_CYAN  "\n%s\n" COLOR_RESET, info);
}
