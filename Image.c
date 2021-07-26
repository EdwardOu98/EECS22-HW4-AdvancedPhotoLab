/* This file contains the        *
 * definitions of functions that *
 * is necessary for loading and  *
 * saving an image               *
 * File created by: Edward Ou    */

#include "Image.h"
#include <stdio.h>
/* Allocate the memory space for the image structure */
/* and return the memory spaces for the color intensity values. */
/* Return the pointer to the image, or NULL in case of error*/
IMAGE *CreateImage(unsigned int Width, unsigned int Height)
{
    IMAGE *i;
    i = malloc(sizeof(IMAGE));
    i->R = malloc(Width*Height*sizeof(unsigned char));
    i->G = malloc(Width*Height*sizeof(unsigned char));
    i->B = malloc(Width*Height*sizeof(unsigned char));

    return i;
}

/* Release the memory spaces for the pixel color intensity values */
/* Deallocate all the memory spaces for the image */
/*Set R/G/B pointers to NULL */
void DeleteImage(IMAGE *image)
{
    free(image->R);
    free(image->G);
    free(image->B);
    free(image);
}

/* Return the image's width in pixels */
unsigned int ImageWidth(const IMAGE *image)
{
    unsigned int Width;

    Width = image->W;

    return Width;
}

/* Return the image's height in pixels */
unsigned int ImageHeight(const IMAGE *image)
{
    unsigned int Height;

    Height = image->H;

    return Height;
}

/* Get the color intensity of the Red Channel of pixel (x, y) in image */
unsigned char GetPixelR(const IMAGE *image, unsigned int x, unsigned int y)
{
    unsigned char R;
    unsigned int Width = ImageWidth(image);
    unsigned int i;
    i = x + (y * Width);

    R = image->R[i];

    return R;
}

/* Get the color intensity of the Green Channel of pixel (x, y) in image */
unsigned char GetPixelG(const IMAGE *image, unsigned int x, unsigned int y)
{
    unsigned char G;
    unsigned int Width = ImageWidth(image);
    unsigned int i;
    i = x + (y * Width);

    G = image->G[i];

    return G;
}

/* Get the color intensity of the Blue Channel of pixel (x, y) in image */
unsigned char GetPixelB(const IMAGE *image, unsigned int x, unsigned int y)
{
    unsigned char B;
    unsigned int Width = ImageWidth(image);
    unsigned int i;
    i = x + (y * Width);

    B = image->B[i];

    return B;
}

/* Set the color intensity of the Red channel of pixel (x, y) in image to r */
void SetPixelR(IMAGE *image, unsigned int x, unsigned int y, unsigned char r)
{
    unsigned int Width = ImageWidth(image);
    unsigned int i;
    i = x + (y * Width);
    
    image->R[i] = r;
}

/* Set the color intensity of the Green channel of pixel (x, y) in image to g */
void SetPixelG(IMAGE *image, unsigned int x, unsigned int y, unsigned char g)
{
    unsigned int Width = ImageWidth(image);  
    unsigned int i;
    i = x + (y * Width);

    image->G[i] = g;
}

/* Set the color intensity of the Blue channel of pixel (x, y) in image to b */
void SetPixelB(IMAGE *image, unsigned int x, unsigned int y, unsigned char b)
{
    unsigned int Width = ImageWidth(image); 
    unsigned int i;
    i = x + (y * Width);

    image->B[i] = b;
}
