#include "Advanced.h"


#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

/* Adjust the brightness of an image */
IMAGE *Brightness(IMAGE *image, int brightness) {
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);
 
        unsigned int Width, Height;
        Width = ImageWidth(image);
        Height = ImageHeight(image);


	for (int i=0; i<Width; i++){
		for (int j=0; j<Height; j++){
			if (GetPixelR(image, i, j) + brightness > 255){
				SetPixelR(image, i, j, 255);
			}else if(GetPixelR(image, i, j) + brightness < 0){
				SetPixelR(image, i, j, 0);
			}else{
				SetPixelR(image, i, j, GetPixelR(image, i, j)+brightness);
			}

			if (GetPixelG(image, i, j) + brightness > 255){
				SetPixelG(image, i, j, 255);
			}else if(GetPixelG(image, i, j) + brightness < 0){
				SetPixelG(image, i, j, 0);
			}else{
				SetPixelG(image, i, j, GetPixelG(image, i, j)+brightness);
			}

			if (GetPixelB(image, i, j) + brightness > 255){
				SetPixelB(image, i, j, 255);
			}else if(GetPixelB(image, i, j) + brightness < 0){
				SetPixelB(image, i, j, 0);
			}else{
				SetPixelB(image, i, j, GetPixelB(image, i, j)+brightness);
			}
		}
	}

    return image;
}

/* Mirror an image horizontally */
IMAGE *HMirror(IMAGE *image) {
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);

        unsigned int Width, Height;
        Width = ImageWidth(image);
        Height = ImageHeight(image);

	int x, y;

    for (y = 0; y < Height; y++) {
        for (x = 0; x < Width / 2; x++) {
            SetPixelR(image, Width-1-x, y, GetPixelR(image, x, y));
            SetPixelG(image, Width-1-x, y, GetPixelG(image, x, y));
            SetPixelB(image, Width-1-x, y, GetPixelB(image, x, y));
		}
	}

    return image;
}

void matrix_3_3multiplyVector_3(float vector[3], const float matrix[3][3])
{
	/* Memory for result */
	float result[3] = {0, 0, 0};

	/* Perform multiplication */
	result[0] = matrix[0][0]*vector[0] + matrix[0][1]*vector[1] + matrix[0][2]*vector[2];
	result[1] = matrix[1][0]*vector[0] + matrix[1][1]*vector[1] + matrix[1][2]*vector[2];
	result[2] = matrix[2][0]*vector[0] + matrix[2][1]*vector[1] + matrix[2][2]*vector[2];

	/* Copy result into other stack's memory */
	vector[0] = result[0];
	vector[1] = result[1];
	vector[2] = result[2];
}

IMAGE *HueRotate(IMAGE *image, float angle)
{
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);
	/* Matrix to transform normalized
	// RGB color vector from the RGB
	// color space to the YIQ color space */
	static const float RGBtoYIQMatrix[3][3] = {{0.299,  0.587,  0.114},
								  			   {0.596, -0.274, -0.321},
								  			   {0.211, -0.523,  0.311}};

	/* Matrix to transform normalized
	// YIQ color vector from the YIQ
	// color space to the RGB color space */
	static const float YIQtoRGBMatrix[3][3] = {{1.000,  0.956,  0.621},
								  			   {1.000, -0.272, -0.647},
								  			   {1.000, -1.107,  1.705}};

	const float cs = cos(angle), ss = sin(angle);

	/* Matrix to perform a hue
	// rotation on a normalized
	// YIQ color vector*/
	const float XYPlaneVectorRotateMatrix[3][3] = {{1.000, 0.000, 0.000},
												   {0.000, cs, -ss},
												   {0.000, ss, cs}};

        unsigned int Width, Height;
        Width = ImageWidth(image);
        Height = ImageHeight(image);


	/* Image variables */
	float temp[3];

	for(unsigned int i = 0; i < Width; i++)
	{
		for(unsigned int j = 0; j < Height; j++)
		{
			/* Create a normalized RGB color
			// vector fro the current pixel
			// in the image */
			temp[0] = GetPixelR(image, i, j)/255.0;
			temp[1] = GetPixelG(image, i, j)/255.0;
			temp[2] = GetPixelB(image, i, j)/255.0;

			/* Transform the RGB vector
			// to a YIQ vector */
			matrix_3_3multiplyVector_3(temp, RGBtoYIQMatrix);

			/* Perform the DIP */
			matrix_3_3multiplyVector_3(temp, XYPlaneVectorRotateMatrix);

			/* Transform back to RGB */
			matrix_3_3multiplyVector_3(temp, YIQtoRGBMatrix);

			/* Denormalize and store back into the image */
			temp[0] = temp[0] * 255;
			temp[0] = temp[0] < 0 ? 0 : temp[0];
			temp[0] = temp[0] > 255 ? 255 : temp[0];
			temp[1] = temp[1] * 255;
			temp[1] = temp[1] < 0 ? 0 : temp[1];
			temp[1] = temp[1] > 255 ? 255 : temp[1];
			temp[2] = temp[2] * 255;
			temp[2] = temp[2] < 0 ? 0 : temp[2];
			temp[2] = temp[2] > 255 ? 255 : temp[2];

			SetPixelR(image, i, j, (unsigned char)temp[0]);
			SetPixelG(image, i, j, (unsigned char)temp[1]);
			SetPixelB(image, i, j, (unsigned char)temp[2]);
		}
	}

    return image;
}

/* Rotate or flip an image */
IMAGE *Rotate(IMAGE *image, int option)
{
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);
    unsigned int x, y, i, j, Width, Height;
    IMAGE *outimage = NULL;
    if(option == 1 || option == 2){
        Width = ImageWidth(image);
        Height = ImageHeight(image);

        outimage = CreateImage(Width, Height);
        outimage->W = Width;
        outimage->H = Height;

        switch(option){
            case 1:
                for(y = 0; y < Height; y++)
                {
                    for(x = 0; x < Width; x++)
                    {
                        i = Width - 1 - x;
                        SetPixelR(outimage, i, y, GetPixelR(image, x, y));
                        SetPixelG(outimage, i, y, GetPixelG(image, x, y));
                        SetPixelB(outimage, i, y, GetPixelB(image, x, y));
                    }
                }
                DeleteImage(image);
                image = NULL;
                break;
            case 2:
                for(y = 0; y < Height; y++)
                {
                    j = Height - 1 - y;
                    for(x = 0; x < Width; x++)
                    {
                        i = Width - 1 - x;
                        SetPixelR(outimage, i, j, GetPixelR(image, x, y));
                        SetPixelG(outimage, i, j, GetPixelG(image, x, y));
                        SetPixelB(outimage, i, j, GetPixelB(image, x, y));
                    }
                }
                DeleteImage(image);
                image = NULL;
                break;
        }
    }
    else if(option == 3 || option == 4){
        Width = ImageHeight(image);
        Height = ImageWidth(image);

        outimage = CreateImage(Width, Height);
        outimage->W = Width;
        outimage->H = Height;

        switch(option){
            case 3:
                for(y = 0; y < ImageHeight(image); y++)
                {
                    i = Width - 1 - y;
                    j = 0;
                    for(x = 0; x < ImageWidth(image); x++)
                    {
                        SetPixelR(outimage, i, j, GetPixelR(image, x, y));
                        SetPixelG(outimage, i, j, GetPixelG(image, x, y));
                        SetPixelB(outimage, i, j, GetPixelB(image, x, y));
                        j++;
                    }
                }
                DeleteImage(image);
                image = NULL;
                break;
            case 4:
                for(y = 0; y < ImageHeight(image); y++)
                {
                    i = y;
                    for(x = 0; x < ImageWidth(image); x++)
                    {
                        j = Height - 1 - x;
                        SetPixelR(outimage, i, j, GetPixelR(image, x, y));
                        SetPixelG(outimage, i, j, GetPixelG(image, x, y));
                        SetPixelB(outimage, i, j, GetPixelB(image, x, y));
                    }
                }
                DeleteImage(image);
                image = NULL;
                break;
        }
    }

    return outimage;
}

/* Change saturation of an image */
IMAGE *Saturate(IMAGE *image, float percent)
{
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);
    int Rt, Gt, Bt;
    int tpR, tpG, tpB;
    unsigned int x, y, Width, Height;

    Width = ImageWidth(image);
    Height = ImageHeight(image);

    for(y = 0; y < Height; y++)
    {
        for(x = 0; x < Width; x++)
        {
            tpR = GetPixelR(image, x, y);
            tpG = GetPixelG(image, x, y);
            tpB = GetPixelB(image, x, y);

            Rt = (tpR + tpG + tpB) / 3;
            Gt = Rt;
            Bt = Rt;

            Rt = tpR - Rt;
            Gt = tpG - Gt;
            Bt = tpB - Bt;

            tpR = tpR + (Rt * (percent/100.00));
            tpG = tpG + (Gt * (percent/100.00));
            tpB = tpB + (Bt * (percent/100.00));

            if(tpR > 255){
                SetPixelR(image, x, y, 255);
            }
            else if(tpR < 0){
                SetPixelR(image, x, y, 0);
            }
            else{
                SetPixelR(image, x, y, tpR);
            }

            if(tpG > 255){
                SetPixelG(image, x, y, 255);
            }
            else if(tpG < 0){
                SetPixelG(image, x, y, 0);
            }
            else{
                SetPixelG(image, x, y, tpG);
            }

            if(tpB > 255){
                SetPixelB(image, x, y, 255);
            }
            else if(tpB < 0){
                SetPixelB(image, x, y, 0);
            }
            else{
                SetPixelB(image, x, y, tpB);
            }
        }
    }

    return image;
}

/* Resize an image */
IMAGE *Resize(IMAGE *image, float percent)
{
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);
    unsigned int Wold, Hold, Wnew, Hnew;
    unsigned int x, y, i, j;
    unsigned char Rt, Gt, Bt;
    int sumR, sumG, sumB;
    unsigned int counter;
    IMAGE *outimage = NULL;

    Wold = ImageWidth(image);
    Hold = ImageHeight(image);
    Wnew = Wold * (percent / 100.00);
    Hnew = Hold * (percent / 100.00);

    outimage = CreateImage(Wnew, Hnew);
    outimage->W = Wnew;
    outimage->H = Hnew;

    if(percent > 100.00)
    {
        for(j = 0; j < Hnew; j++)
        {
            for(i = 0; i < Wnew; i++)
            {
                 x = i / (percent / 100.00);
                 y = j / (percent / 100.00);

                 Rt = GetPixelR(image, x, y);
                 Gt = GetPixelG(image, x, y);
                 Bt = GetPixelB(image, x, y);

                 SetPixelR(outimage, i, j, Rt);
                 SetPixelG(outimage, i, j, Gt);
                 SetPixelB(outimage, i, j, Bt);

            }
        }
    }
    else if(percent < 100.00)
    {
        for(j = 0; j < Hnew; j++)
        {
            for(i = 0; i < Wnew; i++)
            {
                counter = 0;
                sumR = 0;
                sumG = 0;
                sumB = 0;
                for(y = j/(percent/100.00); y < j/(percent/100.00)+(percent/100.00); y++)
                {
                    for(x = i/(percent/100.00); x < i/(percent/100.00)+(percent/100.00); x++)
                    {
                        sumR += GetPixelR(image, x, y);
                        sumG += GetPixelG(image, x, y);
                        sumB += GetPixelB(image, x, y);
                        counter++;
                    }
                }

                Rt = sumR / counter;
                Gt = sumG / counter;
                Bt = sumB / counter;

                SetPixelR(outimage, i, j, Rt);
                SetPixelG(outimage, i, j, Gt);
                SetPixelB(outimage, i, j, Bt);
            }
        }
    }

    DeleteImage(image);
    image = NULL;

    return outimage;
}
/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
