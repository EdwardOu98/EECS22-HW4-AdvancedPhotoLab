#include "FileIO.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

IMAGE *LoadImage(const char *fname) {
    FILE           *File;
    char            Type[SLEN];
    int             Width, Height, MaxValue;

    unsigned int    x, y;
    unsigned char   tR, tG, tB;
    char            ftype[] = ".ppm";
    char            fname_tmp[SLEN];      
    strcpy(fname_tmp, fname);
    strcat(fname_tmp, ftype);
    IMAGE *image = NULL;

    File = fopen(fname_tmp, "r");

    if (!File) {
        printf("Cannot open file \"%s\" for reading!\n", fname);
        return NULL;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
        printf("\nUnsupported file format!\n");
        return NULL;
    }

    fscanf(File, "%d", &Width);
    fscanf(File, "%d", &Height);
    image = CreateImage(Width, Height);
    image->W = Width;
    image->H = Height;


    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255) {
        printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return NULL;
    }
    if ('\n' != fgetc(File)) {
        printf("\nCarriage return expected!\n");
        return NULL;
    }
    for (y = 0; y < Height; y++)
        for (x = 0; x < Width; x++) {
            tR = fgetc(File);
            tG = fgetc(File);
            tB = fgetc(File);
            SetPixelR(image, x, y, tR);
            SetPixelG(image, x, y, tG);
            SetPixelB(image, x, y, tB);
        }
    if (ferror(File)) {
        printf("\nFile error while reading from file!\n");
        return NULL;
    }
    printf("%s was read.\n", fname_tmp);
    fclose(File);

    return image;
}

int SaveImage(const char *fname, const IMAGE *image) {
    FILE	*File;
    int	x, y;
    char	SysCmd[SLEN * 5];
    unsigned int Width, Height;
    char	ftype[] = ".ppm";
    char	fname_tmp[SLEN];  /*avoid to modify on the original pointer, 11/10/10, X.Han*/
    char	fname_tmp2[SLEN];

    strcpy(fname_tmp, fname);
    strcpy(fname_tmp2, fname);
    strcat(fname_tmp2, ftype);

    Width = ImageWidth(image);
    Height = ImageHeight(image);

    File = fopen(fname_tmp2, "w");
    if (!File) {
        printf("Cannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", Width, Height);
    fprintf(File, "255\n");

    for (y = 0; y < Height; y++)
        for (x = 0; x < Width; x++) {
            fputc(GetPixelR(image, x, y), File);
            fputc(GetPixelG(image, x, y), File);
            fputc(GetPixelB(image, x, y), File);
        }

    if (ferror(File)) {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved. \n", fname_tmp2);

    /*
     * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
     * and make it world readable
     */
    sprintf(SysCmd, "/users/grad2/doemer/eecs22/bin/pnmtojpeg_hw3.tcsh %s", fname_tmp2);
    if (system(SysCmd) != 0) {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored.\n", fname_tmp);
    return 0;
}
/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
